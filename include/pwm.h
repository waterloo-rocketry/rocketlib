#ifndef ROCKETLIB_PWM_H
#define ROCKETLIB_PWM_H

#include <stdint.h>

#include "common.h"

#ifdef __cplusplus
#error "C++ is not supported"
#endif

// Structure to hold the configuration details for a PWM pin
typedef struct {
    volatile uint8_t *tris_reg; // Pointer to TRIS register
    volatile uint8_t *pps_reg;  // Pointer to PPS register
    uint8_t pin;                // Pin number (0-7)
} pwm_pin_config_t;

// Macro to concatenate tokens for register naming
// This macro concatenates three tokens together, used for constructing register names dynamically
#define CONCAT(a, b, c) a##b##c

// Macro to get the CCPR Low register based on module number
// This macro forms the register name for the low byte of the Compare/Capture/PWM register
#define CCPR_L(module) CONCAT(CCPR, module, L)

// Macro to get the CCPR High register based on module number
// This macro forms the register name for the high byte of the Compare/Capture/PWM register
#define CCPR_H(module) CONCAT(CCPR, module, H)

// Macro to get the CCPxCON register based on module number
// This macro forms the register name for the control register of the specified CCP module
#define CCP_CON(module) CONCAT(CCP, module, CON)

// Function prototypes
w_status_t pwm_init(
    uint8_t ccp_module, pwm_pin_config_t pin_config, uint16_t pwm_period
); // Initializes the PWM for a specific CCP module

/**
 * Example usage:
 * 
 * pwm_pin_config_t config;
 * config.tris_reg = &TRISA;  // Direct register pointer 
 * config.pps_reg = &RA0PPS;  // Direct register pointer for PPS
 * config.pin = 0;            // Pin number (0-7)
 * 
 * pwm_init(1, config, 255);  // Initialize PWM on CCP1 with period 255
 */

w_status_t pwm_update_duty_cycle(
    uint8_t ccp_module, uint16_t duty_cycle
); // Updates the duty cycle of the specified CCP module

#endif /* ROCKETLIB_PWM_H */
