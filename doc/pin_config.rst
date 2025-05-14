Peripheral Pin Select (PPS) Configuration
******************************************

Overview
========

The Peripheral Pin Select (PPS) module allows connecting peripheral inputs and outputs to 
different physical I/O pins on the PIC18F26K83. This module provides a clean interface 
for setting up all the necessary PPS configurations at initialization time.

Functions
=========

.. c:function:: w_status_t pin_init(void)

   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if PPS initialization was successful

   Initialize Peripheral Pin Select (PPS) configurations for all peripherals.
   
   This function configures the appropriate PPS settings for:
   
   * I2C1 (SCL on RC3, SDA on RC4)
   * SPI1 in master mode (SCK on RB1, SDI on RB2, SDO on RB3)
   * UART1 (TX on RC6, RX on RC7)
   
   Note that PWM/CCP PPS configuration is handled by the PWM module itself when :c:func:`pwm_init` is called.
   
   This function should be called after basic MCU initialization but before any peripheral initialization.

Implementation Details
=====================================================

Helper Functions
-------------------

The implementation includes several helper functions for managing PPS configuration:

.. c:function:: static w_status_t pps_unlock(void)

   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if unlock operation was successful
   
   Internal function that unlocks the PPS registers for configuration. Per the 
   PIC18F26K83 datasheet, modifying PPS registers requires a specific unlock sequence.

.. c:function:: static w_status_t pps_lock(void)

   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if lock operation was successful
   
   Internal function that locks the PPS registers after configuration to prevent
   accidental changes.

.. c:function:: static w_status_t pps_input(volatile unsigned char* pps_register, unsigned char port_pin_code)

   :param pps_register: Pointer to the peripheral's PPS register
   :param port_pin_code: Code representing the port and pin (format: 0bPPPNNN where PPP=port, NNN=pin)
   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if mapping was successful
   :retval: ``W_INVALID_PARAM`` if pps_register is NULL
   
   Helper function for configuring peripheral input pin mappings.

.. c:function:: static w_status_t pps_output(volatile unsigned char* rxy_pps_register, unsigned char peripheral_code)

   :param rxy_pps_register: Pointer to the pin's PPS register
   :param peripheral_code: Code representing the peripheral output function
   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if mapping was successful
   :retval: ``W_INVALID_PARAM`` if rxy_pps_register is NULL
   
   Helper function for configuring peripheral output pin mappings.

Peripheral Configuration Functions
-------------------------------------------------

.. c:function:: static w_status_t pps_configure_i2c1(void)

   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   
   Configures I2C1 to use RC3 for SCL and RC4 for SDA. Since I2C is bidirectional,
   both input and output mappings are configured for each pin. The pins are also set to
   open-drain mode as required for I2C operation.

.. c:function:: static w_status_t pps_configure_spi1(void)

   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   
   Configures the SPI1 module in master mode using:
   
   * RB1 for SCK (output)
   * RB2 for SDI (input)
   * RB3 for SDO (output)

.. c:function:: static w_status_t pps_configure_uart1(void)

   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   
   Configures the UART1 module using:
   
   * RC6 for TX (output)
   * RC7 for RX (input)

.. c:function:: static w_status_t pps_configure_timer_clk(unsigned char timer, unsigned char port, unsigned char pin)

   :param timer: Timer number to configure (0, 1, 3, 5)
   :param port: Port number where clock input is located (0=PORTA, 1=PORTB, 2=PORTC)
   :param pin: Pin number within the port (0-7)
   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   :retval: ``W_INVALID_PARAM`` if timer, port, or pin parameters are invalid
   
   Configures an external clock input for the specified timer. This is not called by default
   in the initialization routine but is available for applications that need external timer
   clock sources.

Usage Example
=============

The PPS configuration should be performed early in your application's initialization sequence:

.. code-block:: c

   int main(void) {
       // Initialize the microcontroller (clocks, etc.)
       mcu_init();
       
       // Configure PPS mapping for peripherals
       w_status_t status = pin_init();
       if (status != W_SUCCESS) {
           // Handle error
       }
       
       // Now initialize peripherals that depend on PPS configuration
       i2c_init(frequency_divider);
       spi_init(mode, clock_polarity, clock_phase);
       // ...other peripheral initializations...
       
       while(1) {
           // Main application loop
       }
   }

Notes
=====

* The actual pin assignments can be modified in the `pin_config.c` file if different pins are required.
* PWM/CCP pin assignments should be configured through the PWM module's own initialization function.
* When switching between peripherals that share the same pins (e.g., SPI and I2C on the same MSSP module),
  the PPS configuration must be updated accordingly. 