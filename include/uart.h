#ifndef ROCKETLIB_UART_H
#define ROCKETLIB_UART_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

/*
 * Initializes the UART1 module.
 *
 * baud_rate: Desired baud rate (e.g., 9600, 115200)
 * fosc: Clock frequency (must be 12 MHz or 48 MHz)
 * enable_flow_control: true to enable RTS flow control, false to disable
 *
 * Returns: W_SUCCESS if setup is successful, otherwise W_INVALID_PARAM
 */
w_status_t uart_init(uint32_t baud_rate, uint32_t fosc, bool enable_flow_control);

/*
 * Transmits a single byte over UART (blocking).
 *
 * waits until the transmit buffer is empty, then sends the byte.
 */
void uart_transmit_byte(uint8_t byte);
// void uart_transmit_buffer(uint8_t *tx, uint8_t len);

/*
 * Interrupt handler for UART1.
 *
 * Handles RX and optional TX,
 * and calls user-defined uart_rx_callback() on received bytes.
 */
void uart_interrupt_handler(void);

/*
 * Internal callback function called when a byte is received via UART1.
 *
 * This is implemented in uart.c and currently echoes the received byte back.
 * It is not intended to be overridden by user code due to MPLAB limitations.
 */
void uart_rx_callback(uint8_t byte);

#endif /* ROCKETLIB_UART_H */

