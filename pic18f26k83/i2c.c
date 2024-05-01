#include <xc.h>

#include "i2c.h"

void i2c_init(void) {
    // CSTR Enable clocking; S Cleared by hardware after Start; MODE 7-bit address; EN enabled; RSEN
    // disabled;
    I2C1CON0 = 0x84;
    // TXU 0; CSD Clock Stretching enabled; ACKT 0; RXO 0; ACKDT Acknowledge; ACKSTAT ACK received;
    // ACKCNT Not Acknowledge;
    I2C1CON1 = 0x80;
    // ABD enabled; GCEN disabled; ACNT disabled; SDAHT 300 ns hold time; BFRET 8 I2C Clock pulses;
    // FME disabled;
    I2C1CON2 = 0x00;
    // Setup clock reference to be base (500khz)/32
    CLKRCON = 0b10010011;
    CLKRCLK = 0b00000011;
    // CLK clock reference
    I2C1CLK = 0x04;
    I2C1PIR = 0; //    ;Clear all the error flags
    I2C1ERR = 0;
}

bool i2c_write(uint8_t address, uint8_t *data, uint8_t len) {
    I2C1ADB1 = (uint8_t)(address << 1);
    I2C1CNT = len;
    I2C1PIRbits.PCIF = 0;
    I2C1ERRbits.NACKIF = 0;
    I2C1CON0bits.S = 1;
    while (len--) {
        I2C1TXB = *data++;
        while (!I2C1STAT1bits.TXBE && !I2C1ERRbits.NACKIF)
            ;
        if (I2C1ERRbits.NACKIF) {
            break;
        }
    }
    while (!I2C1PIRbits.PCIF)
        ;
    I2C1PIRbits.PCIF = 0;
    I2C1STAT1bits.CLRBF = 1;

    return (I2C1ERR & 0x70) != 0;
}

bool i2c_read(uint8_t address, uint8_t *data, uint8_t len) {
    I2C1ADB1 = (uint8_t)((address << 1) | 1);
    I2C1CNT = len;
    I2C1PIRbits.PCIF = 0;
    I2C1ERRbits.NACKIF = 0;
    I2C1CON0bits.S = 1;
    while (len--) {
        while (!I2C1STAT1bits.RXBF && !I2C1ERRbits.NACKIF)
            ;
        if (I2C1ERRbits.NACKIF) {
            break;
        }
        *data++ = I2C1RXB;
    }
    while (!I2C1PIRbits.PCIF)
        ;
    I2C1PIRbits.PCIF = 0;
    I2C1STAT1bits.CLRBF = 1;

    return (I2C1ERR & 0x70) != 0;
}

bool i2c_write_cmd(uint8_t address, uint8_t cmd) {
    return i2c_write(address, &cmd, 1);
}

void i2c_read_reg_multibyte(uint8_t address, uint8_t reg, uint8_t *data, uint8_t len) {
    i2c_write(address, &reg, 1);
    i2c_read(address, data, len);
}

bool i2c_write_reg8(uint8_t address, uint8_t reg, uint8_t val) {
    uint8_t data[2] = {reg, val};
    return i2c_write(address, data, 2);
}

uint8_t i2c_read_reg8(uint8_t address, uint8_t reg) {
    uint8_t data;
    i2c_read_reg_multibyte(address, reg, &data, 1);
    return data;
}

uint16_t i2c_read_reg16(uint8_t address, uint8_t reg) {
    i2c_write(address, &reg, 1);
    uint8_t data[2];
    i2c_read(address, data, 2);
    return (uint16_t)(data[0]) << 8 | data[1];
}
