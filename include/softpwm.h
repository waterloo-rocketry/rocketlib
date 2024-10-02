#ifndef ROCKETLIB_SOFT_PWM_H
#define ROCKETLIB_SOFT_PWM_H

#include <stdint.h>

/* PWM Initialization
   period: Period in 10us
   Note only 10ms period work for now(period=1000)
*/
void pwm_init(uint16_t period);

/* Set PWM Duty Cycle
   duty_cycle: Duty cycle in per-mille(1/1000)
*/
void pwm_set_duty_cycle(uint16_t duty_cycle);

void pwm_enable(void);

void pwm_disable(void);

#endif
