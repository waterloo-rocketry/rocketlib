#include <xc.h>

#include "pwm.h"

uint16_t rocketlib_pwm_count = 0;
uint16_t rocketlib_pwm_period = 1000; // In increment of 10us
uint16_t rocketlib_pwm_negedge_cycle = 0;

void timer2_handle_interrupt(void) {
    rocketlib_pwm_count++;
    if (rocketlib_pwm_count == rocketlib_pwm_period) {
        rocketlib_pwm_count = 0;
    }
    if (rocketlib_pwm_count == 0) {
        LATC5 = 1;
    } else if (rocketlib_pwm_count == rocketlib_pwm_negedge_cycle) {
        LATC5 = 0;
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

    TRISC5 = 0;
    LATC5 = 1;

    PIE4bits.TMR2IE = 1; // enable timer 2 interrupt
}

void pwm_set_duty_cycle(uint16_t duty_cycle) {
    rocketlib_pwm_negedge_cycle = duty_cycle * (rocketlib_pwm_period / 1000);
}

void pwm_enable(void) {}

void pwm_disable(void) {}
