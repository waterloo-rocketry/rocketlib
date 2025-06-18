//#include <pic18f26k83.h> 
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

//#include "canlib.h" //remove -> no canlib dependancy 
//#include "canlib/util/safe_ring_buffer.h"
#include "common.h"
#include "uart.h"

// safe ring buffers for sending and receiving (DELETE)
//static srb_ctx_t rx_buffer; 
//static srb_ctx_t tx_buffer;

// memory pools to use for those srbs. 100 is a completely arbitrary number
//uint8_t rx_buffer_pool[100], tx_buffer_pool[100];

//CALLBACK FXN HERE
__attribute__ ((weak)) void uart_rx_callback (uint8_t byte) {
    //user override fxn 
}

w_status_t uart_init(uint32_t baud_rate, uint32_t fosc, bool enable_flow_control) {
    // only 12 or 48 MHz for pic
    if (fosc != 12000000UL && fosc != 48000000UL) {
        return W_INVALID_PARAM;
    }

    // bool controlled flow control.
    U1CON2bits.FLO = enable_flow_control ? 0b10 : 0b00;

    // low speed
    U1CON0bits.BRGS = 0;
    uint32_t divisor = (U1CON0bits.BRGS == 1) ? 4 : 16;

    // checks if baud rate param within range
    if (baud_rate == 0 || baud_rate > (fosc / divisor)) {
        return W_INVALID_PARAM;
    }

    // don't autodetect baudrate
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

    // we are go for UART
    U1CON1bits.ON = 1;

    // initialize the rx and tx buffers DONT USE BUFFER
    //srb_init(&rx_buffer, rx_buffer_pool, sizeof(rx_buffer_pool), sizeof(uint8_t));
    //srb_init(&tx_buffer, tx_buffer_pool, sizeof(tx_buffer_pool), sizeof(uint8_t));

    // enable receive interrupt
    IPR3bits.U1RXIP = 1;
    PIE3bits.U1RXIE = 1;
    // Do not enable transmit interrupt, that interrupt enable signals that
    // there is data to be sent, which at init time is not true

    return W_SUCCESS;
}

void uart_transmit_byte(uint8_t byte) {
    while (!PIR3bits.U1TXIF);  // blocking wait
    U1TXB = byte;
}

//void uart_transmit_buffer(uint8_t *tx, uint8_t len) { //Implement transmit buffer without canlib dependency 
//    //make into transmit byte fxn
////Make this into a uart transmit byte function
//    // just call uart_transmit_byte with each byte they want us to send
//    while (len--) {
//        //srb_push(&tx_buffer, tx);
//        tx++;
//    }
//    // If the module isn't enabled, give it a byte to send and enable it
//    if (PIE3bits.U1TXIE == 0) {
//        //delete
//        //srb_pop(&tx_buffer, &tx);
//        U1TXB = tx;
//        U1CON0bits.TXEN = 1;
//        // also enable the interrupt for when it's ready to send more data
//        PIE3bits.U1TXIE = 1;
//    }
//}
//DELETE
//bool uart_byte_available(void) {
//    return !srb_is_empty(&rx_buffer);
//}
//DELETE
//uint8_t uart_read_byte(void) {
 //   uint8_t rcv;
  //  srb_pop(&rx_buffer, &rcv);
  //  return rcv;
//}
void uart_interrupt_handler(void) {
    //TX interrupt
    if (PIR3bits.U1TXIF) {
        PIR3bits.U1TXIF = 0;
    }

    //RX interrupt
    if (PIR3bits.U1RXIF) {
        PIR3bits.U1RXIF = 0;
        uint8_t rcv = U1RXB;
        uart_rx_callback(rcv);  // invoke user callback
    }

    //UART error interrupt
    //ignored for now
    if (PIR3bits.U1EIF) {
        PIR3bits.U1EIF = 0;
        // Optionally handle framing/parity/overflow errors
    }

    //global UART interrupt
    if (PIR3bits.U1IF) {
        PIR3bits.U1IF = 0;
    }
}
//for receive, implement a callback option
//void uart_interrupt_handler(void) {
//    if (PIR3bits.U1TXIF) {
//        // check if there are any bytes we still want to transmit
//        if (!srb_is_empty(&tx_buffer)) {
//            // if so, transmit them
//            uint8_t tx;
//            srb_pop(&tx_buffer, &tx);
//            U1TXB = tx;
//        } else {
//            // If we have no data to send, disable this interrupt
//            PIE3bits.U1TXIE = 0;
//            // if not, disable the TX part of the uart module so that TXIF
//            // isn't triggered again and so that we reenable the module on
//            // the next call to uart_transmit_byte
//            U1CON0bits.TXEN = 0;
//        }
//        PIR3bits.U1TXIF = 0;
//    } else if (PIR3bits.U1RXIF) {
//        // we received a byte, need to push into RX buffer and return
//        uint8_t rcv = U1RXB;
//        srb_push(&rx_buffer, &rcv);
//        PIR3bits.U1RXIF = 0;
//    } else if (PIR3bits.U1EIF) {
//        // Some sort of error, ignore for now (TODO?)
//        PIR3bits.U1EIF = 0;
//    } else if (PIR3bits.U1IF) {
//        PIR3bits.U1IF = 0;
//    }
//}
