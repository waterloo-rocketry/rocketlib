#ifndef ROCKETLIB_I2C_H
#define ROCKETLIB_I2C_H

#include <stdbool.h>
#include <stdint.h>

/* Initialize I2C Controller
   I2C Frequency = (100 kHz) / (2 ^ clkdiv)
   Max frequency clkdiv = b000, 100kHz
   Min frequency clkdiv = b111, 781.25 Hz
*/
void i2c_init(uint8_t clkdiv);

bool i2c_write_data(uint8_t address, const uint8_t *data, uint8_t len);
bool i2c_read_data(uint8_t address, uint8_t *data, uint8_t len);
bool i2c_write_reg8(uint8_t address, uint8_t reg, uint8_t val);
bool i2c_write_reg16(uint8_t address, uint8_t reg, uint16_t val);
bool i2c_read_reg8(uint8_t address, uint8_t reg, uint8_t *value);
bool i2c_read_reg16(uint8_t address, uint8_t reg, uint16_t *value);

#endif
