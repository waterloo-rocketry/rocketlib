PIC18 PWM Driver
****************

The driver provides PWM output functionality for the PIC18F26K83 using the CCP (Capture/Compare/PWM) modules. It supports up to four PWM channels, each with configurable pin mapping. Timer 2 is utilized by this driver to manage PWM periods.

Integration
===========

Pin Configuration
-----------------
.. c:macro:: CONCAT(a, b, c)

   Concatenates three tokens to dynamically form register names.

   :param a: First part of the token.
   :param b: Second part of the token.
   :param c: Third part of the token.

.. c:macro:: CCPR_L(module)

   Accesses the CCPR Low register for the specified module.

   :param module: CCP module number (1-4).

.. c:macro:: CCPR_H(module)

   Accesses the CCPR High register for the specified module.

   :param module: CCP module number (1-4).

.. c:macro:: CCP_CON(module)

   Accesses the CCPxCON control register for the specified module.

   :param module: CCP module number (1-4).

.. c:macro:: GET_TRIS_REG(port)

   Retrieves the TRIS register for the specified port.

   :param port: Port letter (A, B, C).

.. c:macro:: GET_PPS_REG(port, pin)

   Retrieves the PPS register address for the specified port and pin.

   :param port: Port letter (A, B, C).
   :param pin: Pin number (0-7).

.. c:macro:: SET_TRIS_OUTPUT(port, pin)

   Sets the specified pin as output by modifying the TRIS register.

   :param port: Port letter (A, B, C).
   :param pin: Pin number (0-7).

.. c:macro:: ASSIGN_PPS(port, pin, ccp_module)

   Assigns the CCP module to the specified PPS register to map the peripheral to the desired pin.

   :param port: Port letter (A, B, C).
   :param pin: Pin number (0-7).
   :param ccp_module: CCP module number (1-4).

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
.. c:macro:: WRITE_DUTY_CYCLE(ccp_module, duty_cycle)

   Write the 10-bit duty cycle value to the appropriate CCPRxH:CCPRxL register pair.

   :param ccp_module: CCP module number (1-4)
   :param duty_cycle: 10-bit duty cycle value (0-1023)

PWM Controller Functions
========================

PWM Pin Configuration Structure
-------------------------------
.. c:type:: pwm_pin_config_t

   Structure that holds the configuration details for a PWM pin.

   :param port: Port letter (A, B, C).
   :param pin: Pin number (0-7).
   :param pps_reg: PPS register value for this pin.

Initialization
--------------
.. c:function:: w_status_t pwm_init(uint8_t ccp_module, pwm_pin_config_t pin_config, uint16_t pwm_period)

   Initializes PWM for the specified CCP module with the given pin configuration and PWM period.

   :param ccp_module: CCP module number (1-4).
   :param pin_config: PWM pin configuration structure containing port, pin, and PPS register values.
   :param pwm_period: PWM period value.
   :return: W_SUCCESS on successful initialization, otherwise an error code.

   This function configures Timer 2, sets the PWM period, and enables the PWM output for the specified CCP module.

PWM Operation
=============

.. c:function:: w_status_t pwm_update_duty_cycle(uint8_t ccp_module, uint16_t duty_cycle)

   Updates the duty cycle of the specified CCP module to the new value.

   :param ccp_module: CCP module number (1-4).
   :param duty_cycle: New duty cycle value (0-1023).
   :return: W_SUCCESS if successful, W_INVALID_PARAM if parameters are out of range.

   The duty cycle is a 10-bit value that determines the percentage of time the signal stays high. The lower 8 bits are written to CCPRxL, and the upper 2 bits are written to CCPRxH.

Timer Configuration
-------------------
.. c:macro:: CONFIGURE_TIMER2(pwm_period)

   Configures Timer 2 to manage PWM periods. The prescaler and postscaler are set to 1:1.

   :param pwm_period: PWM period value to load into the PR2 register.

Helper Functions
================

PPS Configuration
-----------------
.. c:function:: static w_status_t configure_pps(uint8_t ccp_module, pwm_pin_config_t pin_config)

   Configures Peripheral Pin Select (PPS) for the specified pin and CCP module. This function is essential for routing the PWM signal to the correct output pin.

   :param ccp_module: CCP module number (1-4).
   :param pin_config: Structure containing port, pin, and PPS register values.
   :return: W_SUCCESS if successful, W_INVALID_PARAM if the module number is out of range.

Error Handling
==============

Error Codes
-----------
- **W_SUCCESS**: Operation completed successfully.
- **W_INVALID_PARAM**: Parameter value is out of range. Typically returned if an invalid CCP module number or duty cycle value is provided.

Practical Considerations
========================
- **Timer Usage**: Ensure Timer 2 is not shared with other peripherals to avoid conflicts in PWM generation.
- **Prescaler and Postscaler**: The prescaler and postscaler are currently set to 1:1 for simplicity. These can be adjusted to modify the frequency of the PWM signal.
- **Pin Mapping**: Correct pin mapping is critical for proper operation. Incorrect configuration can result in no output or conflicts with other peripherals.
