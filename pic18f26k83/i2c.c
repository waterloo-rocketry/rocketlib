#include <xc.h>

#include "i2c.h"

#ifndef _XTAL_FREQ
// Define crystal frequency for delay macros (default: 16 MHz)
#define _XTAL_FREQ 16000000
#endif

/**
 * @brief Clear I2C buffers and ensure they're in a known state
 *
 * This function clears the I2C buffer flags and flushes any pending data from the receive buffer.
 */
static void clear_i2c_buffers(void) {
	// Clear the buffer flag and flush any pending data
	I2C1STAT1bits.CLRBF = 1;
	while (I2C1STAT1bits.RXBF) {
		(void)I2C1RXB; // Read and discard any data
	}
}

/**
 * @brief Check the current state of the I2C module
 *
 * This function verifies that the I2C module is enabled and checks for any error conditions.
 * If errors are detected, they are cleared before returning.
 *
 * @return w_status_t Returns W_SUCCESS if module is ready, W_IO_ERROR if module is disabled or
 * errors detected
 */
static w_status_t check_i2c_state(void) {
	if (!I2C1CON0bits.EN) {
		return W_IO_ERROR; // Module not enabled
	}

	// Check for any error conditions
	if (I2C1ERRbits.BCLIF || // Bus collision
		I2C1ERRbits.BTOIF || // Bus timeout
		I2C1ERRbits.NACKIF) { // Not acknowledged

		I2C1ERR = 0; // Clear error flags
		return W_IO_ERROR;
	}

	return W_SUCCESS;
}

/**
 * @brief Wait for the I2C bus to become idle
 *
 * This function monitors the Bus Free bit with timeout protection to ensure the I2C bus
 * is available before starting a new transaction.
 *
 * @return w_status_t Returns W_SUCCESS when bus is idle, W_IO_TIMEOUT if timeout occurs
 */
static w_status_t wait_for_idle(void) {
	unsigned int timeout = 0;
	while (!I2C1STAT0bits.BFRE) {
		if (timeout++ >= I2C_POLL_TIMEOUT) {
			return W_IO_TIMEOUT;
		}
	}
	return W_SUCCESS;
}

/**
 * @brief Initialize I2C controller with specified clock settings
 *
 * This function initializes the I2C module in master mode with the specified clock divider.
 * The clock divider determines the I2C bus frequency:
 * - 0 = 100kHz (standard mode)
 * - 1 = 50kHz
 * - 2 = 25kHz
 * - etc.
 *
 * @param clkdiv Clock divider (0-7) that determines I2C frequency
 * @return w_status_t Returns W_SUCCESS on success, W_IO_ERROR if initialization fails
 */
w_status_t i2c_init(uint8_t clkdiv) {
	// Disable module for configuration
	I2C1CON0bits.EN = 0;

	// Setup and verify clock configuration
	CLKRCON = 0b10010000 | (clkdiv & 0x07);
	CLKRCLK = 0b00000011; // MFINTOSC source

	// Wait for clock to stabilize
	unsigned int timeout = 1000;
	while (!CLKRCONbits.CLKRDC && --timeout)
		;
	if (!timeout) {
		return W_IO_ERROR;
	}

	// Select I2C clock source
	I2C1CLK = 0x04; // Clock reference

	// Configure I2C module registers
	I2C1CON0 = 0x04; // 7-bit master mode
	I2C1CON1 = 0x80; // Enable ACK, clock stretching
	I2C1CON2 = 0x00; // Standard timing

	// Clear all flags
	I2C1PIR = 0;
	I2C1ERR = 0;

	// Enable module and verify bus is free
	I2C1CON0bits.EN = 1;
	return wait_for_idle();
}

/**
 * @brief Internal function to write data to an I2C device
 *
 * This is the low-level write function that handles the actual I2C transaction.
 * It verifies bus state, sends the address and data bytes, and waits for completion.
 *
 * @param address I2C device address (7-bit, will be shifted left by 1)
 * @param data Pointer to the data buffer to write
 * @param len Number of bytes to write
 * @return w_status_t Returns W_SUCCESS on success, W_IO_ERROR on error, W_IO_TIMEOUT on timeout
 */
static w_status_t i2c_write(uint8_t address, const uint8_t *data, uint8_t len) {
	// Verify bus state and prepare for transfer
	w_status_t status = check_i2c_state();
	if (status != W_SUCCESS) {
		return status;
	}

	clear_i2c_buffers();
	I2C1PIR = 0;
	I2C1ERR = 0;

	// Configure transfer
	I2C1ADB1 = (uint8_t)(address << 1); // Write address
	I2C1CNT = len;

	// Start transfer
	I2C1CON0bits.S = 1;

	// Send each byte
	while (len--) {
		unsigned int timeout = 0;
		// Wait for transmit buffer to be ready
		while (!I2C1STAT1bits.TXBE) {
			if (I2C1ERRbits.NACKIF || I2C1ERRbits.BCLIF) {
				return W_IO_ERROR;
			}

			// Handle clock stretching
			if (I2C1CON0bits.CSTR) {
				__delay_us(I2C_STRETCH_DELAY);
				timeout = 0;
			} else if (timeout++ >= I2C_POLL_TIMEOUT) {
				return W_IO_TIMEOUT;
			}
		}

		I2C1TXB = *data++;
	}

	// Wait for transfer completion
	unsigned int timeout = 0;
	while (!I2C1PIRbits.PCIF) {
		if (timeout++ >= I2C_POLL_TIMEOUT) {
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

/**
 * @brief Internal function to read data from an I2C device
 *
 * This is the low-level read function that handles the actual I2C transaction.
 * It verifies bus state, sends the read address, receives data bytes, and waits for completion.
 *
 * @param address I2C device address (7-bit, will be shifted left by 1)
 * @param data Pointer to the data buffer to store read data
 * @param len Number of bytes to read
 * @return w_status_t Returns W_SUCCESS on success, W_IO_ERROR on error, W_IO_TIMEOUT on timeout
 */
static w_status_t i2c_read(uint8_t address, uint8_t *data, uint8_t len) {
	// Verify bus state and prepare for transfer
	w_status_t status = check_i2c_state();
	if (status != W_SUCCESS) {
		return status;
	}

	clear_i2c_buffers();
	I2C1PIR = 0;
	I2C1ERR = 0;

	// Configure transfer
	I2C1ADB1 = (uint8_t)((address << 1) | 0x01); // Read address
	I2C1CNT = len;

	// Configure acknowledgment behavior
	I2C1CON1bits.ACKDT = 0; // ACK bytes
	if (len == 1) {
		I2C1CON1bits.ACKCNT = 1; // NACK last byte
	}

	// Start transfer
	I2C1CON0bits.S = 1;

	// Receive each byte
	while (len--) {
		unsigned int timeout = 0;
		// Wait for receive buffer to have data
		while (!I2C1STAT1bits.RXBF) {
			if (I2C1ERRbits.NACKIF || I2C1ERRbits.BCLIF) {
				return W_IO_ERROR;
			}

			// Handle clock stretching
			if (I2C1CON0bits.CSTR) {
				__delay_us(I2C_STRETCH_DELAY);
				timeout = 0;
			} else if (timeout++ >= I2C_POLL_TIMEOUT) {
				return W_IO_TIMEOUT;
			}
		}

		*data++ = I2C1RXB;
	}

	// Wait for transfer completion
	unsigned int timeout = 0;
	while (!I2C1PIRbits.PCIF) {
		if (timeout++ >= I2C_POLL_TIMEOUT) {
			return W_IO_TIMEOUT;
		}
	}

	return W_SUCCESS;
}

/**
 * @brief Write data to an I2C device
 *
 * This function writes a sequence of bytes to the specified I2C device address.
 *
 * @param address I2C device address (7-bit, will be shifted left by 1)
 * @param data Pointer to the data buffer to write
 * @param len Number of bytes to write
 * @return w_status_t Returns W_SUCCESS on success, W_IO_ERROR on error, W_IO_TIMEOUT on timeout
 */
w_status_t i2c_write_data(uint8_t address, const uint8_t *data, uint8_t len) {
	return i2c_write(address, data, len);
}

/**
 * @brief Read data from an I2C device
 *
 * This function reads a sequence of bytes from the specified I2C device address.
 *
 * @param address I2C device address (7-bit, will be shifted left by 1)
 * @param data Pointer to the data buffer to store read data
 * @param len Number of bytes to read
 * @return w_status_t Returns W_SUCCESS on success, W_IO_ERROR on error, W_IO_TIMEOUT on timeout
 */
w_status_t i2c_read_data(uint8_t address, uint8_t *data, uint8_t len) {
	return i2c_read(address, data, len);
}

/**
 * @brief Write an 8-bit value to a register on an I2C device
 *
 * This function writes a single 8-bit value to a specific register address on the I2C device.
 *
 * @param address I2C device address (7-bit, will be shifted left by 1)
 * @param reg Register address to write to
 * @param val 8-bit value to write
 * @return w_status_t Returns W_SUCCESS on success, W_IO_ERROR on error, W_IO_TIMEOUT on timeout
 */
w_status_t i2c_write_reg8(uint8_t address, uint8_t reg, uint8_t val) {
	uint8_t data[2] = {reg, val};
	return i2c_write(address, data, 2);
}

/**
 * @brief Write a 16-bit value to a register on an I2C device
 *
 * This function writes a 16-bit value (MSB first) to a specific register address on the I2C device.
 *
 * @param address I2C device address (7-bit, will be shifted left by 1)
 * @param reg Register address to write to
 * @param val 16-bit value to write (MSB first)
 * @return w_status_t Returns W_SUCCESS on success, W_IO_ERROR on error, W_IO_TIMEOUT on timeout
 */
w_status_t i2c_write_reg16(uint8_t address, uint8_t reg, uint16_t val) {
	uint8_t data[3] = {
		reg,
		(uint8_t)(val >> 8), // MSB first
		(uint8_t)(val & 0xFF) // LSB second
	};
	return i2c_write(address, data, 3);
}

/**
 * @brief Read an 8-bit value from a register on an I2C device
 *
 * This function reads a single 8-bit value from a specific register address on the I2C device.
 *
 * @param address I2C device address (7-bit, will be shifted left by 1)
 * @param reg Register address to read from
 * @param value Pointer to store the read 8-bit value
 * @return w_status_t Returns W_SUCCESS on success, W_IO_ERROR on error, W_IO_TIMEOUT on timeout
 */
w_status_t i2c_read_reg8(uint8_t address, uint8_t reg, uint8_t *value) {
	// Write register address
	w_status_t status = i2c_write(address, &reg, 1);
	if (status != W_SUCCESS) {
		return status;
	}

	// Read register value
	return i2c_read(address, value, 1);
}

/**
 * @brief Read a 16-bit value from a register on an I2C device
 *
 * This function reads a 16-bit value (MSB first) from a specific register address on the I2C
 * device.
 *
 * @param address I2C device address (7-bit, will be shifted left by 1)
 * @param reg Register address to read from
 * @param value Pointer to store the read 16-bit value
 * @return w_status_t Returns W_SUCCESS on success, W_IO_ERROR on error, W_IO_TIMEOUT on timeout
 */
w_status_t i2c_read_reg16(uint8_t address, uint8_t reg, uint16_t *value) {
	// Write register address
	w_status_t status = i2c_write(address, &reg, 1);
	if (status != W_SUCCESS) {
		return status;
	}

	// Read register value
	uint8_t data[2];
	status = i2c_read(address, data, 2);
	if (status != W_SUCCESS) {
		return status;
	}

	// Combine bytes (MSB first)
	*value = ((uint16_t)data[0] << 8) | data[1];
	return W_SUCCESS;
}
