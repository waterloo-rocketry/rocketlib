#ifndef ROCKETLIB_PWM_H
#define ROCKETLIB_PWM_H

#include <stdint.h>

// Macro to configure CCP module pin mapping
#define CONFIGURE_CCP_PIN(ccp_module, output_pin, pps_reg, tris_reg, pps_value)                    \
    do {                                                                                           \
        if (ccp_module == 1 && output_pin == 2) {                                                  \
            pps_reg = pps_value;                                                                   \
            tris_reg = 1;                                                                          \
        } else if (ccp_module == 2 && output_pin == 1) {                                           \
            pps_reg = pps_value;                                                                   \
            tris_reg = 1;                                                                          \
        } else if (ccp_module == 3 && output_pin == 5) {                                           \
            pps_reg = pps_value;                                                                   \
            tris_reg = 1;                                                                          \
        } else if (ccp_module == 4 && output_pin == 0) {                                           \
            pps_reg = pps_value;                                                                   \
            tris_reg = 1;                                                                          \
        }                                                                                          \
    } while (0)

// Macro to configure CCP module mode
#define CONFIGURE_CCP_MODE(ccp_module, ccp_con)                                                    \
    do {                                                                                           \
        if (ccp_module == 1) {                                                                     \
            ccp_con.EN = 0b1;                                                                      \
            ccp_con.FMT = 0b0;                                                                     \
            ccp_con.MODE = 0b1100;                                                                 \
        } else if (ccp_module == 2) {                                                              \
            ccp_con.EN = 0b1;                                                                      \
            ccp_con.FMT = 0b0;                                                                     \
            ccp_con.MODE = 0b1100;                                                                 \
        } else if (ccp_module == 3) {                                                              \
            ccp_con.EN = 0b1;                                                                      \
            ccp_con.FMT = 0b0;                                                                     \
            ccp_con.MODE = 0b1100;                                                                 \
        } else if (ccp_module == 4) {                                                              \
            ccp_con.EN = 0b1;                                                                      \
            ccp_con.FMT = 0b0;                                                                     \
            ccp_con.MODE = 0b1100;                                                                 \
        }                                                                                          \
    } while (0)

// Macro to set the TRIS register for the output pin
#define SET_PWM_OUTPUT_PIN(ccp_module, output_pin)                                                 \
    do {                                                                                           \
        if (ccp_module == 1 && output_pin == 2) {                                                  \
            TRISC2 = 0;                                                                            \
        } else if (ccp_module == 2 && output_pin == 1) {                                           \
            TRISC1 = 0;                                                                            \
        } else if (ccp_module == 3 && output_pin == 5) {                                           \
            TRISB5 = 0;                                                                            \
        } else if (ccp_module == 4 && output_pin == 0) {                                           \
            TRISB0 = 0;                                                                            \
        }                                                                                          \
    } while (0)

// Macro to write the 10-bit duty cycle value to the appropriate CCPRxH:CCPRxL register pair
#define WRITE_DUTY_CYCLE(ccp_module, dutyCycle)                                                    \
    do {                                                                                           \
        if (ccp_module == 1) {                                                                     \
            CCPR1L = dutyCycle & 0xFF;                                                             \
            CCPR1H = (dutyCycle >> 8) & 0x03;                                                      \
        } else if (ccp_module == 2) {                                                              \
            CCPR2L = dutyCycle & 0xFF;                                                             \
            CCPR2H = (dutyCycle >> 8) & 0x03;                                                      \
        } else if (ccp_module == 3) {                                                              \
            CCPR3L = dutyCycle & 0xFF;                                                             \
            CCPR3H = (dutyCycle >> 8) & 0x03;                                                      \
        } else if (ccp_module == 4) {                                                              \
            CCPR4L = dutyCycle & 0xFF;                                                             \
            CCPR4H = (dutyCycle >> 8) & 0x03;                                                      \
        }                                                                                          \
    } while (0)

// Function prototypes
void pwm_init(uint8_t ccp_module, uint8_t output_pin, uint16_t pwm_period);
status_t updatePulseWidth(uint8_t ccp_module, uint16_t dutyCycle);

#endif /* ROCKETLIB_PWM_H */