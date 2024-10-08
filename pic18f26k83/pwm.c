#include "pwm.h"
#include <xc.h>

/*
 * Motor Control and PWM Initialization with Pin Selection
 */

#if (BOARD_UNIQUE_ID == BOARD_ID_CHARGING_AIRBRAKE)
const uint16_t MOTOR_MIN_PULSE_WIDTH_US = 500; // corresponds to 0 deg
const uint16_t MOTOR_MAX_PULSE_WIDTH_US = 2500; // corresponds to 117 deg
#elif (BOARD_UNIQUE_ID == BOARD_ID_CHARGING_PAYLOAD)
const uint16_t MOTOR_MIN_PULSE_WIDTH_US = 1500;
const uint16_t MOTOR_MAX_PULSE_WIDTH_US = 1900;
#endif

// Motor control functions with generalized pin and CCP module support
void pwm_init(uint8_t ccp_module, uint8_t output_pin) {
    // Use the desired output pin RxyPPS control to select CCPx as the source and
    // disable the CCPx pin output driver by setting the associated TRIS bit.
    switch (ccp_module) {
        case 1: // CCP1
            if (output_pin == 2) { // RC2
                RC2PPS = 0b10010; // Map CCP1 to RC2
                TRISC2 = 1; // Set as input initially to disable output driver
            }
            break;

        case 2: // CCP2
            if (output_pin == 1) { // RC1
                RC1PPS = 0b10001; // Map CCP2 to RC1
                TRISC1 = 1; // Set as input initially to disable output driver
            }
            break;

        case 3: // CCP3
            if (output_pin == 5) { // RB5
                RB5PPS = 0b01101; // Map CCP3 to RB5
                TRISB5 = 1; // Set as input initially to disable output driver
            }
            break;

        case 4: // CCP4
            if (output_pin == 0) { // RB0
                RB0PPS = 0b01000; // Map CCP4 to RB0
                TRISB0 = 1; // Set as input initially to disable output driver
            }
            break;

        default:
            // Handle invalid CCP module selection
            return;
    }

    // 2. Load the T2PR register with the PWM period value.
    T2PR = 251;

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
    T2CLK = 0b0001; // Select the timer clock source as FOSC/4
    T2CONbits.CKPS = 0b111; // Set prescale of 128
    T2CONbits.ON = 1; // Enable Timer2

    // 5. Enable PWM output pin
    while (PIR4bits.TMR2IF == 0) {} // Wait until Timer overflows

    switch (output_pin) {
        case 2: // RC2
            TRISC2 = 0; // Set RC2 as output
            break;
        case 1: // RC1
            TRISC1 = 0; // Set RC1 as output
            break;
        case 5: // RB5
            TRISB5 = 0; // Set RB5 as output
            break;
        case 0: // RB0
            TRISB0 = 0; // Set RB0 as output
            break;
    }
}

#if (BOARD_UNIQUE_ID == BOARD_ID_CHARGING_AIRBRAKE || BOARD_UNIQUE_ID == BOARD_ID_CHARGING_PAYLOAD)

void updatePulseWidth(uint8_t percent) {
    uint32_t pulseWidth_us = (uint32_t)(MOTOR_MIN_PULSE_WIDTH_US +
                                        ((float)percent / 100.0) *
                                            (MOTOR_MAX_PULSE_WIDTH_US - MOTOR_MIN_PULSE_WIDTH_US));
    uint32_t bitWrite =
        (uint32_t)((pulseWidth_us * 48) / 128); // 48 is Fosc in MHz, 128 is prescaler
    // write PW/(Tosc * prescale value)
    CCPR3L = bitWrite & 0xFF;
    CCPR3H = (bitWrite >> 8) & 0x03; // honestly not sure about this; it's a rough guess
}

#endif