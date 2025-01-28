#ifndef ROCKETLIB_I2C_H
#define ROCKETLIB_I2C_H

#include "common.h"
#include <stdint.h>

/*
 * Configuration Constants
 * These define the behavior and timing characteristics of the I2C interface
 */
#define I2C_POLL_TIMEOUT 2000 // Maximum cycles to wait during I2C operations
#define I2C_STRETCH_DELAY 100 // Microseconds to delay during clock stretching
#define I2C_CLOCK_FREQ 100000 // Default I2C clock frequency (100 kHz)

/*
 * Initialize I2C controller with specified clock settings
 * clkdiv: Clock divider (0-7) determines I2C frequency:
 *   0 = 100kHz (standard mode)
 *   1 = 50kHz
 *   2 = 25kHz
 *   etc.
 */
w_status_t i2c_init(uint8_t clkdiv);

w_status_t i2c_write_data(uint8_t address, const uint8_t *data, uint8_t len);
w_status_t i2c_read_data(uint8_t address, uint8_t *data, uint8_t len);
w_status_t i2c_write_reg8(uint8_t address, uint8_t reg, uint8_t val);
w_status_t i2c_write_reg16(uint8_t address, uint8_t reg, uint16_t val);
w_status_t i2c_read_reg8(uint8_t address, uint8_t reg, uint8_t *value);
w_status_t i2c_read_reg16(uint8_t address, uint8_t reg, uint16_t *value);

#endif
