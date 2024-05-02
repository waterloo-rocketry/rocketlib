#ifndef ROCKETLIB_PWM_H
#define ROCKETLIB_PWM_H

#include <stdbool.h>
#include <stdint.h>

/* PWM Initialization
   period: Period in 10us
   Note only 10ms period work for now(period=1000)
*/
void pwm_init(uint16_t period);

/* Set PWM Duty Cycle
   duty_cycle: Duty cycle in per-mille(1/1000)
*/
bool pwm_set_duty_cycle(uint16_t duty_cycle);

/* Set PWM Pulse Width
   pulse_width: Pulse width in increment of 10 us
*/
bool pwm_set_pulse_width(uint16_t pulse_width);

void pwm_enable(void);

void pwm_disable(void);

#endif
