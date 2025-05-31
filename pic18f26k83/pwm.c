#include "pwm.h"
#include <xc.h>

/**
 * @brief Get CCPR Low register address for the specified CCP module
 */
static volatile uint8_t* get_ccpr_low_register(uint8_t ccp_module) {
    switch (ccp_module) {
        case 1U: return &CCPR1L;
        case 2U: return &CCPR2L;
        case 3U: return &CCPR3L;
        case 4U: return &CCPR4L;
        case 5U: return &CCPR5L;
        default: return NULL;
    }
}

/**
 * @brief Get CCPR High register address for the specified CCP module
 */
static volatile uint8_t* get_ccpr_high_register(uint8_t ccp_module) {
    switch (ccp_module) {
        case 1U: return &CCPR1H;
        case 2U: return &CCPR2H;
        case 3U: return &CCPR3H;
        case 4U: return &CCPR4H;
        case 5U: return &CCPR5H;
        default: return NULL;
    }
}

/**
 * @brief Get CCPxCON register address for the specified CCP module
 */
static volatile uint8_t* get_ccp_con_register(uint8_t ccp_module) {
    switch (ccp_module) {
        case 1U: return &CCP1CON;
        case 2U: return &CCP2CON;
        case 3U: return &CCP3CON;
        case 4U: return &CCP4CON;
        case 5U: return &CCP5CON;
        default: return NULL;
    }
}

w_status_t pwm_init(uint8_t ccp_module, uint16_t pwm_period) {
    // Validate CCP module
    if (ccp_module < 1U || ccp_module > 5U) {
        return W_INVALID_PARAM;
    }

    // Get the CCPxCON register for this module
    volatile uint8_t *ccp_con = get_ccp_con_register(ccp_module);
    if (ccp_con == NULL) {
        return W_INVALID_PARAM;
    }

    // Enable CCP module in PWM mode
    *ccp_con = 0x8CU; // PWM mode selection

    // Set PWM period using Timer2
    PR2 = (uint8_t)(pwm_period & 0xFFU); // Load lower 8 bits of PWM period into PR2 register
    TMR2 = 0U; // Reset Timer2 count to 0
    T2CONbits.T2CKPS = 0U; // Set Timer2 prescaler to 1:1 (no prescaling)
    T2CONbits.TOUTPS = 0U; // Set Timer2 postscaler to 1:1 (no postscaling)
    T2CONbits.TMR2ON = 1U; // Start Timer2 to begin PWM operation

    // Wait for Timer2 to reach the period value before starting PWM
    while (PIR1bits.TMR2IF == 0U) {
        // Wait until Timer2 overflow flag is set
    }
    PIR1bits.TMR2IF = 0U; // Clear Timer2 interrupt flag to continue

    return W_SUCCESS;
}

w_status_t pwm_update_duty_cycle(uint8_t ccp_module, uint16_t duty_cycle) {
    // Validate CCP module and duty cycle range
    if (ccp_module < 1U || ccp_module > 5U || duty_cycle > 1023U) {
        return W_INVALID_PARAM;
    }

    // Get register addresses
    volatile uint8_t *ccpr_low = get_ccpr_low_register(ccp_module);
    volatile uint8_t *ccpr_high = get_ccpr_high_register(ccp_module);

    if (ccpr_low == NULL || ccpr_high == NULL) {
        return W_INVALID_PARAM;
    }

    // Update the lower 8 bits of the duty cycle
    *ccpr_low = (uint8_t)(duty_cycle & 0xFFU);

    // Update the upper 2 bits of the duty cycle for 10-bit resolution
    *ccpr_high = (uint8_t)((duty_cycle >> 8) & 0x03U);

    return W_SUCCESS;
}
