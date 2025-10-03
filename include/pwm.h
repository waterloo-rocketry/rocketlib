#ifndef ROCKETLIB_PWM_H
#define ROCKETLIB_PWM_H

#include <stdint.h>

#include "common.h"

#ifdef __cplusplus
#error "C++ is not supported"
#endif

// Structure to hold the configuration details for a PWM pin
typedef struct {
	uint8_t port; // Port letter (A, B, C)
	uint8_t pin; // Pin number (0-7)
	uint8_t pps_reg; // PPS register value for this pin
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

// Macro to get the TRIS register based on port letter
// TRIS registers control the direction of pins (input or output)
// This macro dynamically constructs the TRIS register name for a given port
#define GET_TRIS_REG(port) CONCAT(TRIS, port, A)

// Macro to get the PPS register address based on port and pin
// PPS (Peripheral Pin Select) allows mapping of peripherals to different pins
// This macro forms the PPS register name for a given port and pin
#define GET_PPS_REG(port, pin) CONCAT(R, port, pin##PPS)

// Macro to set the TRIS register for a specific pin
// This macro configures the specified pin as an output by clearing the corresponding bit in the
// TRIS register
#define SET_TRIS_OUTPUT(port, pin) (GET_TRIS_REG(port) &= ~(1 << (pin)))

// Macro to assign the CCP module to the PPS register
// This macro assigns the CCP module to a specific pin by writing to the PPS register
#define ASSIGN_PPS(port, pin, ccp_module) (*GET_PPS_REG(port, pin) = (ccp_module))

// Function prototypes
w_status_t pwm_init(uint8_t ccp_module, pwm_pin_config_t pin_config,
					uint16_t pwm_period); // Initializes the PWM for a specific CCP module
w_status_t
pwm_update_duty_cycle(uint8_t ccp_module,
					  uint16_t duty_cycle); // Updates the duty cycle of the specified CCP module

#endif /* ROCKETLIB_PWM_H */
