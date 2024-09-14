PIC18 Software PWM Driver
*************************

The driver provides PWM output functionality for PIC18F26K83 on one pin, with timer only. Pulse-width/duty-cycle can be adjusted when output is enabled. Timer 2 is used by this driver.

Integration
===========

ROCKETLIB_PWM_PIN Macro
-----------------------
Set ``ROCKETLIB_PWM_PIN`` macro to the pin register name for output, e.g. ``LATC6``

ROCKETLIB_PWM_INVERT_POLARITY Macro
-----------------------------------
Set ``ROCKETLIB_PWM_INVERT_POLARITY`` macro to inverse output polarity

Interrupt
---------
``void timer2_handle_interrupt(void)`` have to be called when having a timer 2 interrupt

SoftPWM Controller Functions
============================

Initialization
--------------
.. c:function:: void pwm_init(uint16_t period)

   Initialize Software PWM

   :param uint16_t period: period in increment of 10 us

Enable
------
.. c:function:: void pwm_enable(void)

   Enable PWM Output

Disable
-------
.. c:function:: void pwm_disable(void)

   Disable PWM Output

Set Duty Cycle
--------------
.. c:function:: bool pwm_set_duty_cycle(uint16_t duty_cycle)

   Set duty cycle

   :param uint16_t duty_cycle: Duty cycle in per-mille(1/1000)
   :return: success or not
   :retval true: success
   :retval false: failed

Set Pulse Width
---------------
.. c:function:: bool pwm_set_pulse_width(uint16_t pulse_width)

   Set pulse width

   :param uint16_t pulse_width: pulse width in increment of 10 us
   :return: success or not
   :retval true: success
   :retval false: failed