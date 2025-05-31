#ifndef ROCKETLIB_PIN_CONFIG_H
#define ROCKETLIB_PIN_CONFIG_H

#include "common.h"
#include <stdint.h>
#include <stdbool.h>

// Structure to hold pin configuration details
typedef struct {
    uint8_t port; // Port number (0=A, 1=B, 2=C)
    uint8_t pin;  // Pin number (0-7)
} pin_config_t;

// Structure for I2C pin configuration
typedef struct {
    pin_config_t scl; // SCL pin configuration
    pin_config_t sda; // SDA pin configuration
} i2c_pin_config_t;

// Structure for SPI pin configuration
typedef struct {
    pin_config_t sck; // Clock pin configuration
    pin_config_t sdi; // Data input pin configuration
    pin_config_t sdo; // Data output pin configuration
    pin_config_t ss;  // Slave select pin configuration (optional)
} spi_pin_config_t;

// Structure for UART pin configuration
typedef struct {
    pin_config_t tx; // Transmit pin configuration
    pin_config_t rx; // Receive pin configuration
} uart_pin_config_t;

// Structure for PWM pin configuration
typedef struct {
    pin_config_t output; // PWM output pin configuration
} pwm_pin_config_t;

// Structure for external interrupt pin configuration
typedef struct {
    pin_config_t input; // External interrupt input pin configuration
} ext_int_pin_config_t;

// PPS input and output codes for different peripherals
// I2C PPS codes
#define PPS_I2C1_SCL_OUTPUT 0b100001
#define PPS_I2C1_SDA_OUTPUT 0b100010
#define PPS_I2C2_SCL_OUTPUT 0b100011
#define PPS_I2C2_SDA_OUTPUT 0b100100

// SPI PPS codes
#define PPS_SPI1_SCK_OUTPUT 0b011110
#define PPS_SPI1_SDO_OUTPUT 0b011111
#define PPS_SPI2_SCK_OUTPUT 0b100000
#define PPS_SPI2_SDO_OUTPUT 0b100001

// UART PPS codes
#define PPS_UART1_TX_OUTPUT 0b010011
#define PPS_UART2_TX_OUTPUT 0b010100
#define PPS_UART3_TX_OUTPUT 0b010101
#define PPS_UART4_TX_OUTPUT 0b010110
#define PPS_UART5_TX_OUTPUT 0b010111

// PWM/CCP PPS codes
#define PPS_CCP1_OUTPUT 0b001100
#define PPS_CCP2_OUTPUT 0b001101
#define PPS_CCP3_OUTPUT 0b001110
#define PPS_CCP4_OUTPUT 0b001111
#define PPS_CCP5_OUTPUT 0b010000

/**
 * @brief Configure I2C pins dynamically
 *
 * @param i2c_module I2C module number (1-2)
 * @param pin_config I2C pin configuration structure
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if parameters are invalid
 */
w_status_t pps_configure_i2c(uint8_t i2c_module, i2c_pin_config_t pin_config);

/**
 * @brief Configure SPI pins dynamically for master mode
 *
 * @param spi_module SPI module number (1-2)
 * @param pin_config SPI pin configuration structure
 * @param use_ss Whether to configure slave select pin
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if parameters are invalid
 */
w_status_t pps_configure_spi(uint8_t spi_module, spi_pin_config_t pin_config, bool use_ss);

/**
 * @brief Configure UART pins dynamically
 *
 * @param uart_module UART module number (1-5)
 * @param pin_config UART pin configuration structure
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if parameters are invalid
 */
w_status_t pps_configure_uart(uint8_t uart_module, uart_pin_config_t pin_config);

/**
 * @brief Configure PWM/CCP pins dynamically
 *
 * @param ccp_module CCP module number (1-5)
 * @param pin_config PWM pin configuration structure
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if parameters are invalid
 */
w_status_t pps_configure_pwm(uint8_t ccp_module, pwm_pin_config_t pin_config);

/**
 * @brief Configure external interrupt pins dynamically
 *
 * @param int_number External interrupt number (0-2 for INT0, INT1, INT2)
 * @param pin_config External interrupt pin configuration structure
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if parameters are invalid
 */
w_status_t pps_configure_external_interrupt(uint8_t int_number, ext_int_pin_config_t pin_config);

/**
 * @brief Configure timer external clock input
 *
 * @param timer Timer number (0, 1, 2, 3, 4, 5, 6)
 * @param pin_config Pin configuration for the external clock input
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if parameters are invalid
 */
w_status_t pps_configure_timer_clk(uint8_t timer, pin_config_t pin_config);

/**
 * @brief Configure timer gate input
 *
 * @param timer Timer number (0, 1, 2, 3, 4, 5, 6)
 * @param pin_config Pin configuration for the timer gate input
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if parameters are invalid
 */
w_status_t pps_configure_timer_gate(uint8_t timer, pin_config_t pin_config);

#endif /* ROCKETLIB_PIN_CONFIG_H */