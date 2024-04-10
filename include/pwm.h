#ifndef ROCKETLIB_PWM_H
#define ROCKETLIB_PWM_H

void pwm_init(unsigned int freq, unsigned int period);
void pwm_set_duty_cycle(unsigned int duty_cycle);
void pwm_enable(void);
void pwm_disable(void);

#endif
