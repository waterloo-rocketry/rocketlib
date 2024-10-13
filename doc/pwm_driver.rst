PIC18 PWM Driver
****************

The driver provides PWM output functionality for PIC18F26K83 using the CCP (Capture/Compare/PWM) modules. It supports up to four PWM channels, each with configurable pin mapping. Timer 2 is used by this driver.

Integration
===========

Pin Configuration
-----------------
.. c:macro:: CONFIGURE_CCP_PIN(ccp_module, output_pin, pps_reg, tris_reg, pps_value)

   Configure the CCP module pin mapping.

   :param ccp_module: CCP module number (1-4)
   :param output_pin: Output pin number
   :param pps_reg: PPS register to configure
   :param tris_reg: TRIS register to configure
   :param pps_value: PPS value for the selected CCP module

CCP Mode Configuration
----------------------
.. c:macro:: CONFIGURE_CCP_MODE(ccp_module, ccp_con)

   Configure the CCP module for PWM mode.

   :param ccp_module: CCP module number (1-4)
   :param ccp_con: CCPxCON register to configure

Output Pin Configuration
------------------------
.. c:macro:: SET_PWM_OUTPUT_PIN(ccp_module, output_pin)

   Set the TRIS register for the output pin.

   :param ccp_module: CCP module number (1-4)
   :param output_pin: Output pin number

Duty Cycle Configuration
------------------------
.. c:macro:: WRITE_DUTY_CYCLE(ccp_module, dutyCycle)

   Write the 10-bit duty cycle value to the appropriate CCPRxH:CCPRxL register pair.

   :param ccp_module: CCP module number (1-4)
   :param dutyCycle: 10-bit duty cycle value (0-1023)

PWM Controller Functions
========================

Initialization
--------------
.. c:function:: void pwm_init(uint8_t ccp_module, uint8_t output_pin, uint16_t pwm_period)

   Initialize PWM for the specified CCP module and output pin.

   :param uint8_t ccp_module: CCP module number (1-4)
   :param uint8_t output_pin: Output pin number
   :param uint16_t pwm_period: PWM period value

Update Pulse Width
------------------
.. c:function:: status_t updatePulseWidth(uint8_t ccp_module, uint16_t dutyCycle)

   Update the pulse width (duty cycle) for the specified CCP module.

   :param uint8_t ccp_module: CCP module number (1-4)
   :param uint16_t dutyCycle: New duty cycle value (0-1023)
   :return: W_SUCCESS if successful, W_FAILURE if the duty cycle is out of range
   :rtype: status_t
