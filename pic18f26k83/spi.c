#include <stdbool.h>
#include <xc.h>

#include "spi.h"

void spi1_init(void) {
    TRISC3 = 0;

    SPI1CLKbits.CLKSEL = 0;
    SPI1BAUD = 0x22;

    SPI1CON0bits.MST = 1; // set mode to master
    SPI1CON0bits.BMODE = 1; // sets bit mode to constant width
    SPI1CON1bits.CKP = 0; // idle state for clk is low
    SPI1CON1bits.SSP = 1; // make cs active low
    SPI1CON1bits.SDOP = 0;
    SPI1CON1bits.SDIP = 0;

    SPI1CON1bits.CKE = 1; // change data on falling edge

    SPI1CON2bits.TXR = 0; // no transmit required for transfer
    SPI1CON2bits.RXR = 1; // receive data in FIFO
    SPI1TWIDTH = 0; // 8 bits
    // Enable SPI
    PIE2bits.SPI1RXIE = 1; // enable interrupts so we know when device has ACK
    SPI1CON0bits.EN = 1; // enable SPI
}

/**/
uint8_t spi1_exchange(uint8_t data) {
    INTCON0bits.GIE = 0;
    SPI1CON2bits.TXR = 0;
    SPI1CON2bits.RXR = 1;
    SPI1TCNT = 1;
    SPI1TXB = data; /*if we're sending data, send it. if we're recieving, this is 0 (unless you
   // initialized bad), but makes sure we output a clock*/
    while (!PIR2bits.SPI1RXIF) {
    } // wait until receive buffer is not empty?? (basically until theres a clock)
    data = SPI1RXB;
    INTCON0bits.GIE = 1;
    return data; // reading should remove top most byte
}

void spi1_exchange_buffer(uint8_t *data, uint8_t data_len) {
    // set data length
    SPI1TCNTL = data_len;
    SPI1TCNTH = 0;
    // uint8_t *block = data;
    while (data_len) {
        *data = spi1_exchange(*data); // sends pointer, so data gets shoved into array
        data++; // increment array
        data_len--;
    }
}
