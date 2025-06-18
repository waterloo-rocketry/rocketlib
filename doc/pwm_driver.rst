PIC18 PWM Driver
****************

The driver provides PWM output functionality for the PIC18F26K83 using the CCP (Capture/Compare/PWM) modules. It supports up to five PWM channels (CCP1-CCP5), each with configurable pin mapping through the pin configuration module. Timer 2 is utilized by this driver to manage PWM periods.

Features
========

* Support for CCP modules 1-5 for PWM output generation
* Configurable PWM period and duty cycle
* Timer 2 integration for period management
* Separation of concerns: pin configuration handled by dedicated pin configuration module
* BARR-C compliant implementation with proper error handling

Prerequisites
=============

Before using the PWM driver, you must configure the output pins using the pin configuration module:

.. code-block:: c

   // Configure PWM output pin first
   pwm_pin_config_t pwm_config = {
       .output = {.port = 1, .pin = 5} // RB5 for PWM output
   };
   w_status_t status = pps_configure_pwm(1, pwm_config);
   if (status != W_SUCCESS) {
       // Handle configuration error
   }
   
   // Then initialize PWM module
   status = pwm_init(1, 1000); // CCP1, period = 1000
   if (status != W_SUCCESS) {
       // Handle initialization error
   }

PWM Controller Functions
========================

PWM Pin Configuration
-----------------------

The PWM pin configuration uses the :c:type:`pwm_pin_config_t` structure defined in the pin configuration module. This structure contains a single output pin configuration that specifies where the PWM signal should be routed.

For details on the structure definition, see the :doc:`pin_config` documentation.

Initialization
--------------
.. c:function:: w_status_t pwm_init(uint8_t ccp_module, uint16_t pwm_period)

   Initializes PWM for the specified CCP module with the given PWM period.

   :param ccp_module: CCP module number (1-5).
   :param pwm_period: PWM period value.
   :return: W_SUCCESS on successful initialization, otherwise an error code.

   This function configures Timer 2, sets the PWM period, and enables the PWM output for the specified CCP module.
   
   .. note::
      Pin configuration must be done separately using :c:func:`pps_configure_pwm` from the pin configuration module before calling this function.

PWM Operation
=============

.. c:function:: w_status_t pwm_update_duty_cycle(uint8_t ccp_module, uint16_t duty_cycle)

   Updates the duty cycle of the specified CCP module to the new value.

   :param ccp_module: CCP module number (1-5).
   :param duty_cycle: New duty cycle value (0-1023).
   :return: W_SUCCESS if successful, W_INVALID_PARAM if parameters are out of range.

   The duty cycle is a 10-bit value that determines the percentage of time the signal stays high. The lower 8 bits are written to CCPRxL, and the upper 2 bits are written to CCPRxH.

Timer Configuration
-------------------

The PWM driver automatically configures Timer 2 to manage PWM periods. The prescaler and postscaler are set to 1:1 for optimal resolution. The PWM period is set during initialization via the ``pwm_init()`` function.

Helper Functions
================

PPS Configuration
-----------------

PPS (Peripheral Pin Select) configuration for PWM is handled by the pin configuration module. Use :c:func:`pps_configure_pwm` to configure the output pin before initializing the PWM module.

See the :doc:`pin_config` documentation for details on PWM pin configuration.

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
