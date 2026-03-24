/**
 * @file
 * @brief PIC18 I2C master driver
 *
 * This module provides I2C master functionality for the PIC18F26K83 microcontroller.
 * It supports standard I2C operations including read/write data and register read/write
 * operations for 8-bit and 16-bit registers.
 */

#ifndef ROCKETLIB_I2C_H
#define ROCKETLIB_I2C_H

#include "common.h"
#include <stdint.h>

#ifdef __cplusplus
#error "C++ is not supported"
#endif

/**
 * @brief Configuration constants for I2C interface
 *
 * These define the behavior and timing characteristics of the I2C interface
 */
#define I2C_POLL_TIMEOUT 2000 ///< Maximum cycles to wait during I2C operations
#define I2C_STRETCH_DELAY 100 ///< Microseconds to delay during clock stretching
#define I2C_CLOCK_FREQ 100000 ///< Default I2C clock frequency (100 kHz)

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
w_status_t i2c_init(uint8_t clkdiv);

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
w_status_t i2c_write_data(uint8_t address, const uint8_t *data, uint8_t len);

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
w_status_t i2c_read_data(uint8_t address, uint8_t *data, uint8_t len);

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
w_status_t i2c_write_reg8(uint8_t address, uint8_t reg, uint8_t val);

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
w_status_t i2c_write_reg16(uint8_t address, uint8_t reg, uint16_t val);

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
w_status_t i2c_read_reg8(uint8_t address, uint8_t reg, uint8_t *value);

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
w_status_t i2c_read_reg16(uint8_t address, uint8_t reg, uint16_t *value);

#endif
