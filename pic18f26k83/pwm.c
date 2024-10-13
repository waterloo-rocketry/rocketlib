#include "pwm.h"
#include "common.h"
#include <xc.h>

/*
 * PWM Initialization with Pin Selection
 */

// General PWM control functions with generalized pin and CCP module support
void pwm_init(uint8_t ccp_module, uint8_t output_pin, uint16_t pwm_period) {
    // Use macro to configure the CCP pin mapping
    CONFIGURE_CCP_PIN(ccp_module, output_pin, RC2PPS, TRISC2, 0b10010);
    CONFIGURE_CCP_PIN(ccp_module, output_pin, RC1PPS, TRISC1, 0b10001);
    CONFIGURE_CCP_PIN(ccp_module, output_pin, RB5PPS, TRISB5, 0b01101);
    CONFIGURE_CCP_PIN(ccp_module, output_pin, RB0PPS, TRISB0, 0b01000);

    // 2. Load the T2PR register with the PWM period value.
    // Calculate PWM period using the formula in Equation 23-1:
    // PWM Period = [(T2PR) + 1] * 4 * Tosc * (TMR2 Prescale Value)
    T2PR = pwm_period; // Set T2PR value based on the desired PWM period.
                       // The PWM frequency is the inverse of this period.

    // 3. Use macro to configure the CCP module for PWM mode
    CONFIGURE_CCP_MODE(ccp_module, CCP1CONbits);
    CONFIGURE_CCP_MODE(ccp_module, CCP2CONbits);
    CONFIGURE_CCP_MODE(ccp_module, CCP3CONbits);
    CONFIGURE_CCP_MODE(ccp_module, CCP4CONbits);

    // 4. Configure and start Timer2
    PIR4bits.TMR2IF = 0; // Clear the TMR2IF interrupt flag bit
    T2CLK = 0b0001; // Select the timer clock source as FOSC/4
    T2CONbits.CKPS = 0b111; // Set prescale of 128
    T2CONbits.ON = 1; // Enable Timer2

    // 5. Enable PWM output pin
    while (PIR4bits.TMR2IF == 0) {} // Wait until Timer overflows

    // Use macro to set the TRIS register for the output pin
    SET_PWM_OUTPUT_PIN(ccp_module, output_pin);
}

status_t updatePulseWidth(uint8_t ccp_module, uint16_t dutyCycle) {
    // Ensure the duty cycle value is within the 10-bit range (0 to 1023)
    if (dutyCycle > 1023) {
        return W_FAILURE;
    }

    // Use macro to write the 10-bit duty cycle value to the appropriate CCPRxH:CCPRxL register pair
    WRITE_DUTY_CYCLE(ccp_module, dutyCycle);

    return W_SUCCESS;
}
