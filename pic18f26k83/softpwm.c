#include <stdbool.h>

#include <xc.h>

#include "softpwm.h"

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
                ROCKETLIB_PWM_PIN = 0;
                rocketlib_pwm_requested_disable = false;
                rocketlib_pwm_enabled = false;
                return;
            }
        }
        if (rocketlib_pwm_count == 0) {
            ROCKETLIB_PWM_PIN = 1;
        } else if (rocketlib_pwm_count == rocketlib_pwm_negedge_cycle) {
            ROCKETLIB_PWM_PIN = 0;
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

    ROCKETLIB_PWM_PIN = 0;

    PIE4bits.TMR2IE = 1; // enable timer 2 interrupt
}

void pwm_set_duty_cycle(uint16_t duty_cycle) {
    rocketlib_pwm_negedge_cycle = duty_cycle * (rocketlib_pwm_period / 1000);
}

void pwm_enable(void) {
    rocketlib_pwm_enabled = true;
}

void pwm_disable(void) {
    rocketlib_pwm_requested_disable = true;
}
