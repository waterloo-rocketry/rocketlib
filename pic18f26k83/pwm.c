#include "pwm.h"
#include <xc.h>

// Helper function to configure PPS registers using direct register access
static w_status_t configure_pps(uint8_t ccp_module, pwm_pin_config_t pin_config) {
	volatile uint8_t *pps_reg;

	// Ensure the CCP module number is within valid range (1-4)
	if (ccp_module < 1 || ccp_module > 4) {
		return W_INVALID_PARAM; // Return error if the module number is out of range
	}

	// Set the pin as output to drive PWM signal
	// This macro modifies the TRIS register to set the specified pin as an output
	SET_TRIS_OUTPUT(pin_config.port, pin_config.pin);
    // Set the pin as output to drive PWM signal directly using the TRIS register
    *pin_config.tris_reg &= ~(1 << pin_config.pin);

    // Assign the CCP module to the corresponding PPS register directly
    *pin_config.pps_reg = ccp_module;

	return W_SUCCESS; // Return success status after configuring PPS
}

// Initialize PWM for a specific CCP module
w_status_t pwm_init(uint8_t ccp_module, pwm_pin_config_t pin_config, uint16_t pwm_period) {
	// Configure PPS registers to map CCP module to the selected pin
	w_status_t status = configure_pps(ccp_module, pin_config);
	if (status != W_SUCCESS) {
		return status; // Return error status if PPS configuration fails
	}

	// Obtain the address of the CCPxCON register using macro
	volatile uint8_t *ccp_con = &CCP_CON(ccp_module);
	*ccp_con = 0x8C; // Enable CCP module in PWM mode (PWM mode selection)

	// Set PWM period using Timer2
	PR2 = pwm_period & 0xFF; // Load lower 8 bits of PWM period into PR2 register
	TMR2 = 0; // Reset Timer2 count to 0
	T2CONbits.T2CKPS = 0; // Set Timer2 prescaler to 1:1 (no prescaling)
	T2CONbits.TOUTPS = 0; // Set Timer2 postscaler to 1:1 (no postscaling)
	T2CONbits.TMR2ON = 1; // Start Timer2 to begin PWM operation

	// Wait for Timer2 to reach the period value before starting PWM
	while (!PIR1bits.TMR2IF) {} // Wait until Timer2 overflow flag is set
	PIR1bits.TMR2IF = 0; // Clear Timer2 interrupt flag to continue

	return W_SUCCESS; // Return success status after PWM initialization
}

// Update the duty cycle of a specific CCP module
w_status_t pwm_update_duty_cycle(uint8_t ccp_module, uint16_t duty_cycle) {
	// Validate CCP module and duty cycle range
	if (ccp_module < 1 || ccp_module > 4 || duty_cycle > 1023) {
		return W_INVALID_PARAM; // Return error if module number or duty cycle is out of range
	}

	// Update the lower 8 bits of the duty cycle
	// This sets the low byte of the duty cycle for the PWM signal
	CCPR_L(ccp_module) = duty_cycle & 0xFF;

	// Update the upper 2 bits of the duty cycle for 10-bit resolution
	// This sets the high bits of the duty cycle to achieve 10-bit PWM precision
	CCPR_H(ccp_module) = (duty_cycle >> 8) & 0x03;

	return W_SUCCESS; // Return success status after updating duty cycle
}
