#include "pwm.h"
#include <xc.h>

/*
 * PWM Initialization with Pin Selection
 */

// General PWM control functions with generalized pin and CCP module support
void pwm_init(uint8_t ccp_module, uint8_t output_pin) {
    // 1. Configure the CCP pin mapping
    switch (ccp_module) {
        case 1: // CCP1
            if (output_pin == 2) { // RC2
                RC2PPS = 0b10010;  // Map CCP1 to RC2
                TRISC2 = 1;         // Set as input initially to disable output driver
            }
            break;
        case 2: // CCP2
            if (output_pin == 1) { // RC1
                RC1PPS = 0b10001;  // Map CCP2 to RC1
                TRISC1 = 1;         // Set as input initially to disable output driver
            }
            break;
        case 3: // CCP3
            if (output_pin == 5) { // RB5
                RB5PPS = 0b01101;  // Map CCP3 to RB5
                TRISB5 = 1;         // Set as input initially to disable output driver
            }
            break;
        case 4: // CCP4
            if (output_pin == 0) { // RB0
                RB0PPS = 0b01000;  // Map CCP4 to RB0
                TRISB0 = 1;         // Set as input initially to disable output driver
            }
            break;
        default:
            // Handle invalid CCP module selection
            return;
    }

    // 2. Load the T2PR register with the PWM period value.
    // Calculate PWM period using the formula in Equation 23-1: PWM Period = [(T2PR) + 1] * 4 * Tosc * (TMR2 Prescale Value)
    T2PR = 5; // Adjust this value as needed based on required PWM frequency

    // 3. Configure the CCP module for PWM mode
    switch (ccp_module) {
        case 1:
            CCP1CONbits.EN = 0b1; // enable CCP1
            CCP1CONbits.FMT = 0b0; // Set to right-aligned
            CCP1CONbits.MODE = 0b1100; // set to PWM operation
            break;
        case 2:
            CCP2CONbits.EN = 0b1; // enable CCP2
            CCP2CONbits.FMT = 0b0; // Set to right-aligned
            CCP2CONbits.MODE = 0b1100; // set to PWM operation
            break;
        case 3:
            CCP3CONbits.EN = 0b1; // enable CCP3
            CCP3CONbits.FMT = 0b0; // Set to right-aligned
            CCP3CONbits.MODE = 0b1100; // set to PWM operation
            break;
        case 4:
            CCP4CONbits.EN = 0b1; // enable CCP4
            CCP4CONbits.FMT = 0b0; // Set to right-aligned
            CCP4CONbits.MODE = 0b1100; // set to PWM operation
            break;
    }

    // 4. Configure and start Timer2
    PIR4bits.TMR2IF = 0; // Clear the TMR2IF interrupt flag bit
    T2CLK = 0b0001;      // Select the timer clock source as FOSC/4
    T2CONbits.CKPS = 0b111; // Set prescale of 128
    T2CONbits.ON = 1;    // Enable Timer2

    // 5. Enable PWM output pin
    while (PIR4bits.TMR2IF == 0) {} // Wait until Timer overflows

    switch (ccp_module) {
        case 1:
            if (output_pin == 2) { // RC2
                TRISC2 = 0; // Set RC2 as output
            }
            break;
        case 2:
            if (output_pin == 1) { // RC1
                TRISC1 = 0; // Set RC1 as output
            }
            break;
        case 3:
            if (output_pin == 5) { // RB5
                TRISB5 = 0; // Set RB5 as output
            }
            break;
        case 4:
            if (output_pin == 0) { // RB0
                TRISB0 = 0; // Set RB0 as output
            }
            break;
    }
}

void updatePulseWidth(uint8_t ccp_module, uint16_t dutyCycle) {
    // Ensure the duty cycle value is within the 10-bit range (0 to 1023)
    if (dutyCycle > 1023) {
        dutyCycle = 1023;
    }

    // Write the 10-bit duty cycle value to the appropriate CCPRxH:CCPRxL register pair based on ccp_module
    switch (ccp_module) {
        case 1:
            CCPR1L = dutyCycle & 0xFF; // Lower 8 bits
            CCPR1H = (dutyCycle >> 8) & 0x03; // Upper 2 bits
            break;
        case 2:
            CCPR2L = dutyCycle & 0xFF; // Lower 8 bits
            CCPR2H = (dutyCycle >> 8) & 0x03; // Upper 2 bits
            break;
        case 3:
            CCPR3L = dutyCycle & 0xFF; // Lower 8 bits
            CCPR3H = (dutyCycle >> 8) & 0x03; // Upper 2 bits
            break;
        case 4:
            CCPR4L = dutyCycle & 0xFF; // Lower 8 bits
            CCPR4H = (dutyCycle >> 8) & 0x03; // Upper 2 bits
            break;
    }
}