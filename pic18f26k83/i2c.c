#include <xc.h>

#include "i2c.h"

#ifndef I2C_POLL_TIMEOUT
#define I2C_POLL_TIMEOUT 2000
#endif

w_status_t i2c_init(uint8_t clkdiv) {
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
    CLKRCON = 0b10010000 | clkdiv;
    CLKRCLK = 0b00000011;
    // CLK clock reference
    I2C1CLK = 0x04;
    I2C1PIR = 0; //    ;Clear all the error flags
    I2C1ERR = 0;

    return W_SUCCESS;
}

static w_status_t i2c_write(uint8_t address, const uint8_t *data, uint8_t len) {
    I2C1ADB1 = (uint8_t)(address << 1);
    I2C1CNT = len;
    I2C1PIRbits.PCIF = 0;
    I2C1ERRbits.NACKIF = 0;
    I2C1CON0bits.S = 1;

    unsigned int timeout;
    while (len--) {
        I2C1TXB = *data++;
        timeout = 0;
        while (!I2C1STAT1bits.TXBE && !I2C1ERRbits.NACKIF) {
            if (timeout >= I2C_POLL_TIMEOUT) {
                return W_IO_TIMEOUT;
            }
            timeout++;
        }
        if (I2C1ERRbits.NACKIF) {
            return W_IO_ERROR;
        }
    }

    timeout = 0;
    while (!I2C1PIRbits.PCIF) {
        if (timeout >= I2C_POLL_TIMEOUT) {
            return W_IO_TIMEOUT;
        }
        timeout++;
    }

    I2C1PIRbits.PCIF = 0;
    I2C1STAT1bits.CLRBF = 1;

    if ((I2C1ERR & 0x70) != 0) {
        return W_IO_ERROR;
    }

    return W_SUCCESS;
}

static w_status_t i2c_read(uint8_t address, uint8_t *data, uint8_t len) {
    I2C1ADB1 = (uint8_t)((address << 1) | 1);
    I2C1CNT = len;
    I2C1PIRbits.PCIF = 0;
    I2C1ERRbits.NACKIF = 0;
    I2C1CON0bits.S = 1;

    unsigned int timeout;
    while (len--) {
        timeout = 0;
        while (!I2C1STAT1bits.RXBF && !I2C1ERRbits.NACKIF) {
            if (timeout >= I2C_POLL_TIMEOUT) {
                return W_IO_TIMEOUT;
            }
            timeout++;
        }
        if (I2C1ERRbits.NACKIF) {
            return W_IO_ERROR;
        }
        *data++ = I2C1RXB;
    }

    timeout = 0;
    while (!I2C1PIRbits.PCIF) {
        if (timeout >= I2C_POLL_TIMEOUT) {
            return W_IO_TIMEOUT;
        }
        timeout++;
    }
    I2C1PIRbits.PCIF = 0;
    I2C1STAT1bits.CLRBF = 1;

    if ((I2C1ERR & 0x70) != 0) {
        return W_IO_ERROR;
    }

    return W_SUCCESS;
}

w_status_t i2c_write_data(uint8_t address, const uint8_t *data, uint8_t len) {
    return i2c_write(address, data, len);
}

w_status_t i2c_read_data(uint8_t address, uint8_t *data, uint8_t len) {
    return i2c_read(address, data, len);
}

w_status_t i2c_write_reg8(uint8_t address, uint8_t reg, uint8_t val) {
    uint8_t data[2] = {reg, val};
    return i2c_write(address, data, 2);
}

w_status_t i2c_write_reg16(uint8_t address, uint8_t reg, uint16_t val) {
    uint8_t data[3] = {reg, (uint8_t)(val >> 8), (uint8_t)val};
    return i2c_write(address, data, 3);
}

w_status_t i2c_read_reg8(uint8_t address, uint8_t reg, uint8_t *value) {
    w_status_t status = i2c_write(address, &reg, 1);
    if (status != W_SUCCESS) {
        return W_IO_ERROR;
    }

    uint8_t data;
    status = i2c_read(address, &data, 1);
    if (status != W_SUCCESS) {
        return W_IO_ERROR;
    }
    *value = data;
    return W_SUCCESS;
}

w_status_t i2c_read_reg16(uint8_t address, uint8_t reg, uint16_t *value) {
    w_status_t status = i2c_write(address, &reg, 1);
    if (status != W_SUCCESS) {
        return W_IO_ERROR;
    }
    uint8_t data[2];
    status = i2c_read(address, data, 2);
    if (status != W_SUCCESS) {
        return W_IO_ERROR;
    }
    *value = (uint16_t)(data[0]) << 8 | data[1];
    return W_SUCCESS;
}
