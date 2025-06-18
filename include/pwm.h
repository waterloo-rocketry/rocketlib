#ifndef ROCKETLIB_PWM_H
#define ROCKETLIB_PWM_H

#include "common.h"
#include <stdint.h>

/**
 * @brief Initialize PWM for a specific CCP module
 *
 * This function only handles PWM initialization. Pin configuration must be done
 * separately using the pin_config module before calling this function.
 *
 * @param ccp_module CCP module number (1-5)
 * @param pwm_period PWM period value
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if parameters are invalid
 */
w_status_t pwm_init(uint8_t ccp_module, uint16_t pwm_period);

/**
 * @brief Update the duty cycle of a specific CCP module
 *
 * @param ccp_module CCP module number (1-5)
 * @param duty_cycle 10-bit duty cycle value (0-1023)
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if parameters are invalid
 */
w_status_t pwm_update_duty_cycle(uint8_t ccp_module, uint16_t duty_cycle);

#endif /* ROCKETLIB_PWM_H */
