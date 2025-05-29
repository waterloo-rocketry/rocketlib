#ifndef UART_H_
#define UART_H_

#include "common.h"
#include <stdbool.h>
#include <stdint.h>

/*
 * Initialize UART module. Set up rx and tx buffers, set up module,
 * and enable the requisite interrupts
 */
w_status_t uart_init(uint32_t baud_rate, uint32_t fosc, bool enable_flow_control);

/*
 * A lot like transmitting a single byte, except there are multiple bytes. tx does
 * not need to be null terminated, that's why we have the len parameter
 *
 * tx: pointer to an array of bytes to send
 * len: the number of bytes that should be sent from that array
 */
void uart_transmit_buffer(uint8_t *tx, uint8_t len);

/*
 * returns true if there's a byte waiting to be read from the UART module
 */
bool uart_byte_available(void);

/*
 * pops a byte from the receive buffer and returns it. Don't call this
 * function unless uart_byte_available is returning true. Don't call this
 * function from an interrupt context.
 */
uint8_t uart_read_byte(void);

/*
 * handler for all UART1 module interrputs. That is, PIR3:U1IF, U1EIF, U1TXIF, and U1RXIF
 * this function clears the bits in PIR3 that it handles.
 */
void uart_interrupt_handler(void);

#endif /* UART_H */

