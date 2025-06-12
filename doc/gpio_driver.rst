GPIO Driver
***********

The GPIO driver provides basic digital input/output functionality for the PIC18F26K83. This driver focuses on high-level GPIO operations, while the underlying pin configuration (direction, digital/analog mode, PPS mapping) is handled by the dedicated pin configuration module.

**Key Features:**

* Simple digital I/O macros for performance-critical operations
* Support for all available GPIO pins (PORTA, PORTB, PORTC)
* Macro-based implementation for zero runtime overhead
* Separation of concerns: pin mode configuration handled separately

**Current Status:** Interface defined, implementation pending

Relationship with Pin Configuration
====================================

The GPIO driver operates at a higher level than the pin configuration module:

* **Pin Configuration Module**: Handles pin direction (TRIS), digital/analog mode (ANSEL), and PPS routing
* **GPIO Driver**: Provides convenient macros for reading/writing pin values after configuration

**Setup Sequence:**

1. Configure pin using pin configuration module functions
2. Use GPIO macros for fast I/O operations

.. code-block:: c

   // Step 1: Configure pin for digital output (done once)
   pin_config_t led_pin = {.port = 1, .pin = 5}; // RB5
   configure_pin_digital(led_pin, 0); // 0 = output
   
   // Step 2: Use GPIO macros for fast operations (done repeatedly)
   gpio_set_output(B, 5, 1); // Turn on LED
   gpio_set_output(B, 5, 0); // Turn off LED

GPIO Macros
===========

Output Control
--------------
.. c:macro:: gpio_enable_output(port, pin)

   Initialize a GPIO pin for digital output operation.

   :param port: MCU port letter (A, B, or C)
   :param pin: Pin number (0-7)
   
   **Example:**
   
   .. code-block:: c
   
      gpio_enable_output(B, 5); // Configure RB5 as output

.. c:macro:: gpio_set_output(port, pin, value)

   Set a GPIO digital output pin to the specified value.

   :param port: MCU port letter (A, B, or C)
   :param pin: Pin number (0-7)
   :param value: Output value (0 = low, 1 = high)
   
   **Example:**
   
   .. code-block:: c
   
      gpio_set_output(C, 2, 1); // Set RC2 high
      gpio_set_output(C, 2, 0); // Set RC2 low

Input Control
-------------
.. c:macro:: gpio_enable_input(port, pin)

   Initialize a GPIO pin for digital input operation.

   :param port: MCU port letter (A, B, or C)
   :param pin: Pin number (0-7)
   
   **Example:**
   
   .. code-block:: c
   
      gpio_enable_input(A, 3); // Configure RA3 as input

.. c:macro:: gpio_get_input(port, pin)

   Read the current state of a digital input pin.

   :param port: MCU port letter (A, B, or C)
   :param pin: Pin number (0-7)
   :returns: Pin state (0 = low, 1 = high)
   
   **Example:**
   
   .. code-block:: c
   
      if (gpio_get_input(A, 0)) {
          // RA0 is high
      }

Usage Examples
==============

Basic GPIO Operations
---------------------

.. code-block:: c

   #include "gpio.h"
   
   int main(void) {
       // Initialize MCU
       mcu_init();
       
       // Configure LED output
       gpio_enable_output(B, 5); // RB5 as LED output
       
       // Configure button input
       gpio_enable_input(A, 0);  // RA0 as button input
       
       while(1) {
           // Read button state and control LED
           if (gpio_get_input(A, 0)) {
               gpio_set_output(B, 5, 1); // Button pressed, LED on
           } else {
               gpio_set_output(B, 5, 0); // Button not pressed, LED off
           }
       }
   }

Integration with Pin Configuration Module
------------------------------------------

For more complex scenarios requiring PPS routing or special pin configurations:

.. code-block:: c

   #include "pin_config.h"
   #include "gpio.h"
   
   int main(void) {
       mcu_init();
       
       // Complex example: Configure pins for various functions
       
       // 1. Regular GPIO (no PPS needed)
       gpio_enable_output(C, 0); // Simple LED
       gpio_enable_input(A, 1);  // Simple button
       
       // 2. Pins that need PPS configuration first
       // Configure PWM output pin, then use as GPIO if needed
       pwm_pin_config_t pwm_config = {
           .output = {.port = 1, .pin = 6} // RB6
       };
       pps_configure_pwm(1, pwm_config);
       // Now RB6 is configured for PWM, but could be used as GPIO too
       
       // 3. External interrupt pins
       ext_int_pin_config_t int_config = {
           .input = {.port = 1, .pin = 0} // RB0
       };
       pps_configure_external_interrupt(0, int_config);
       // RB0 is now configured for INT0, hardware handles the input
       
       while(1) {
           // Simple GPIO operations
           gpio_set_output(C, 0, 1);
           delay_ms(500);
           gpio_set_output(C, 0, 0);
           delay_ms(500);
       }
   }

Blinking LED Example
---------------------

.. code-block:: c

   #include "gpio.h"
   #include "millis.h"
   
   #define LED_PORT C
   #define LED_PIN  2
   #define BLINK_INTERVAL_MS 1000
   
   int main(void) {
       mcu_init();
       timer0_init(); // For millis() function
       
       // Configure LED pin
       gpio_enable_output(LED_PORT, LED_PIN);
       
       uint32_t last_toggle = 0;
       uint8_t led_state = 0;
       
       while(1) {
           uint32_t current_time = millis();
           
           if (current_time - last_toggle >= BLINK_INTERVAL_MS) {
               led_state = !led_state;
               gpio_set_output(LED_PORT, LED_PIN, led_state);
               last_toggle = current_time;
           }
       }
   }

Design Rationale
================

Why Macros?
-----------

The GPIO driver uses macros instead of functions for performance reasons:

**Performance Benefits:**
* **Zero Runtime Overhead**: Macros expand to direct register operations
* **Compile-time Optimization**: Port and pin selections resolved at compile time
* **Optimal Code Generation**: Single instruction operations where possible

**Example Expansion:**

.. code-block:: c

   // This macro call:
   gpio_set_output(B, 3, 1);
   
   // Expands to something like:
   PORTBbits.RB3 = 1;
   
   // Which generates a single assembly instruction

**Alternative Approaches:**

Function-based GPIO would require runtime overhead:

.. code-block:: c

   // Hypothetical function call:
   gpio_set_pin(GPIO_PORTB, 3, 1);
   
   // Would require:
   // - Function call overhead
   // - Parameter passing
   // - Runtime port/pin selection logic
   // - Multiple assembly instructions

Macro Parameter Format
-----------------------

The macro parameters use port letters (A, B, C) rather than numbers for clarity:

* **More Readable**: ``gpio_set_output(B, 5, 1)`` vs ``gpio_set_output(1, 5, 1)``
* **Hardware Correlation**: Matches datasheet naming (RB5, RC2, etc.)
* **Compile-time Validation**: Invalid port letters cause immediate compile errors

Architecture Integration
=========================

Layer Responsibilities
-----------------------

.. code-block:: text

   Application Layer
         |
   +-----v-----+
   | GPIO      |  <- High-level I/O operations
   | Driver    |
   +-----------+
         |
   +-----v-----+
   | Pin       |  <- Pin configuration and PPS routing
   | Config    |
   +-----------+
         |
   +-----v-----+
   | Hardware  |  <- Register-level operations
   | Registers |
   +-----------+

**Clear Separation:**
* **GPIO Driver**: Fast I/O operations for application logic
* **Pin Config**: One-time setup of pin modes and routing
* **Hardware Layer**: Direct register manipulation

Implementation Status
======================

**Current Status:** Interface defined, implementation pending

**Planned Implementation:**
The macros will be implemented to generate optimal assembly code for each supported port and pin combination.

**Future Enhancements:**
* Port-wide operations (read/write entire ports)
* Atomic bit manipulation functions
* Pin change interrupt integration
* Pull-up/pull-down configuration support

**Integration Notes:**
* Designed to work seamlessly with the pin configuration module
* Compatible with all PPS-routed peripherals
* Suitable for both simple GPIO and complex mixed-mode applications
