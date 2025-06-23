#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#include "common.h"
#include "uart.h"

void uart_rx_callback(uint8_t byte) {
    uart_transmit_byte(byte); // echos back recieved byte
}

w_status_t uart_init(uint32_t baud_rate, uint32_t fosc, bool enable_flow_control) {
    // make sure to Configure UART1 pins before running!!!

    // only 12 or 48 MHz support for pic
    if (fosc != 12000000UL && fosc != 48000000UL) {
        return W_INVALID_PARAM;
    }

    // bool controlled flow control.
    U1CON2bits.FLO = enable_flow_control ? 0b10 : 0b00;

    // low speed
    U1CON0bits.BRGS = 0;
    uint32_t divisor = (U1CON0bits.BRGS == 1) ? 4 : 16;

    // checks for valid baud rate
    if (baud_rate == 0 || baud_rate > (fosc / divisor)) {
        return W_INVALID_PARAM;
    }

    // disable autodetect baudrate
    U1CON0bits.ABDEN = 0;
    // normal mode (8 bit, no parity, no 9th bit)
    U1CON0bits.MODE = 0;
    // enable transmit
    U1CON0bits.TXEN = 1;
    // enable receive
    U1CON0bits.RXEN = 1;

    // keep running on overflow, never stop receiving
    U1CON2bits.RUNOVF = 1;

    // dynamic baud rate
    uint16_t brg = (fosc / (divisor * baud_rate)) - 1;
    U1BRGH = (brg >> 8) & 0xFF;
    U1BRGL = brg & 0xFF;

    // enable UART module
    U1CON1bits.ON = 1;

    // enable receive interrupt
    IPR3bits.U1RXIP = 1;
    PIE3bits.U1RXIE = 1;

    // do not enable TX interrupt initially; only needed when there's queued data

    return W_SUCCESS;
}

void uart_transmit_byte(uint8_t byte) {
    while (!PIR3bits.U1TXIF)
        ; // blocking wait
    U1TXB = byte;
}

void uart_interrupt_handler(void) {
    // TX interrupt
    if (PIR3bits.U1TXIF) {
        PIR3bits.U1TXIF = 0;
    }

    // RX interrupt
    if (PIR3bits.U1RXIF) {
        PIR3bits.U1RXIF = 0;
        uint8_t rcv = U1RXB;
        uart_rx_callback(rcv); // invoke user callback
    }

    // UART error interrupt
    if (PIR3bits.U1EIF) {
        PIR3bits.U1EIF = 0;
        // optionally handle framing/parity/overflow errors
    }

    // global UART interrupt
    if (PIR3bits.U1IF) {
        PIR3bits.U1IF = 0;
    }
}

