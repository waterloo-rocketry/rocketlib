#ifndef ROCKETLIB_PIN_CONFIG_H
#define ROCKETLIB_PIN_CONFIG_H

#include "common.h"

/**
 * @brief Initializes the Peripheral Pin Select (PPS) configuration
 *
 * This function sets up all PPS mappings for peripherals including I2C, SPI, UART, and Timer
 * inputs. Note that PWM/CCP PPS configuration is handled by the PWM module itself when pwm_init()
 * is called.
 *
 * Should be called after basic MCU initialization (mcu_init) but before peripheral initialization.
 *
 * @return w_status_t W_SUCCESS if successful
 */
w_status_t pin_init(void);

#endif /* ROCKETLIB_PIN_CONFIG_H */