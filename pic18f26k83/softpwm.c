#include <stdbool.h>

#include <xc.h>

#include "pwm.h"

#ifndef ROCKETLIB_PWM_PIN
#error "ROCKETLIB_PWM_PIN need to be defined as pin register name of PWM output, e.g. LATC5"
#endif

uint16_t rocketlib_pwm_count = 0;
uint16_t rocketlib_pwm_period = 1000; // In increment of 10us
uint16_t rocketlib_pwm_negedge_cycle = 0;

volatile bool rocketlib_pwm_enabled = false;
volatile bool rocketlib_pwm_requested_disable = false;

void timer2_handle_interrupt(void) {
    if (rocketlib_pwm_enabled) {
        rocketlib_pwm_count++;
        if (rocketlib_pwm_count == rocketlib_pwm_period) {
            rocketlib_pwm_count = 0;
            if (rocketlib_pwm_requested_disable) {
#ifndef ROCKETLIB_PWM_INVERT_POLARITY
                ROCKETLIB_PWM_PIN = 0;
#else
                ROCKETLIB_PWM_PIN = 1;
#endif
                rocketlib_pwm_requested_disable = false;
                rocketlib_pwm_enabled = false;
                return;
            }
        }
        if ((rocketlib_pwm_count == 0) && (rocketlib_pwm_negedge_cycle != 0)) {
#ifndef ROCKETLIB_PWM_INVERT_POLARITY
            ROCKETLIB_PWM_PIN = 1;
#else
            ROCKETLIB_PWM_PIN = 0;
#endif
        } else if (rocketlib_pwm_count == rocketlib_pwm_negedge_cycle) {
#ifndef ROCKETLIB_PWM_INVERT_POLARITY
            ROCKETLIB_PWM_PIN = 0;
#else
            ROCKETLIB_PWM_PIN = 1;
#endif
        }
    }
}

void pwm_init(uint16_t period) {
    // T2CS FOSC/4;
    T2CLKCON = 0x01;

    // T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not
    // Synchronized;
    T2HLT = 0x00;

    // T2RSEL T2CKIPPS pin;
    T2RST = 0x00;

    // PR2 119;
    T2PR = 0x77;

    // TMR2 0;
    T2TMR = 0x00;

    // Clearing IF flag.
    PIR4bits.TMR2IF = 0;

    // T2CKPS 1:1; T2OUTPS 1:1; TMR2ON on;
    T2CON = 0x80;

    rocketlib_pwm_period = period;

#ifndef ROCKETLIB_PWM_INVERT_POLARITY
    ROCKETLIB_PWM_PIN = 0;
#else
    ROCKETLIB_PWM_PIN = 1;
#endif

    PIE4bits.TMR2IE = 1; // enable timer 2 interrupt
}

bool pwm_set_duty_cycle(uint16_t duty_cycle) {
    if (duty_cycle > 1000) {
        return false;
    }
    rocketlib_pwm_negedge_cycle = (uint32_t)duty_cycle * rocketlib_pwm_period / 1000;
    return true;
}

bool pwm_set_pulse_width(uint16_t pulse_width) {
    if (pulse_width > rocketlib_pwm_period) {
        return false;
    }
    rocketlib_pwm_negedge_cycle = pulse_width;
    return true;
}

void pwm_enable(void) {
    rocketlib_pwm_enabled = true;
}

void pwm_disable(void) {
    rocketlib_pwm_requested_disable = true;
}
