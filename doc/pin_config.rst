Peripheral Pin Select (PPS) Configuration
******************************************

Overview
========

The Peripheral Pin Select (PPS) module provides centralized pin configuration for all digital 
peripherals on the PIC18F26K83. This module handles pin configuration for I2C, SPI, UART, PWM,
Timer, and External Interrupt peripherals, following modular programming principles with clear 
separation of concerns.

**Key Features:**

* Centralized pin configuration for all digital peripherals
* Support for multiple peripheral instances (I2C1/2, UART1-5, SPI1/2, CCP1-5, Timer0-6)
* Automatic PPS register lock/unlock management with interrupt safety
* Robust error handling with comprehensive cleanup patterns
* BARR-C coding standard compliance (no function-like macros)
* Modular design promoting separation of concerns

The pin configuration module is responsible for:

* Pin direction (input/output) configuration
* Digital mode selection  
* Open-drain configuration for I2C
* PPS input and output mappings
* Multiple module support (I2C1/2, UART1-5, etc.)
* Interrupt-safe PPS register management

Other peripheral modules (PWM, I2C, SPI, UART) focus purely on their functionality and rely on this
module for pin configuration.

.. note::
   This module handles only digital peripherals.


Data Structures
===============

Pin Configuration Structure
---------------------------
.. c:type:: pin_config_t

   Structure that holds basic pin configuration details.

   .. c:member:: uint8_t port
   
      Port number (0=PORTA, 1=PORTB, 2=PORTC).
      
   .. c:member:: uint8_t pin
   
      Pin number (0-7).

I2C Pin Configuration Structure
-------------------------------
.. c:type:: i2c_pin_config_t

   Structure that holds I2C pin configuration details.

   .. c:member:: pin_config_t scl
   
      SCL (Serial Clock) pin configuration structure.
      
   .. c:member:: pin_config_t sda
   
      SDA (Serial Data) pin configuration structure.

SPI Pin Configuration Structure
-------------------------------
.. c:type:: spi_pin_config_t

   Structure that holds SPI pin configuration details.

   .. c:member:: pin_config_t sck
   
      SCK (Serial Clock) pin configuration structure.
      
   .. c:member:: pin_config_t sdi
   
      SDI (Serial Data Input) pin configuration structure.
      
   .. c:member:: pin_config_t sdo
   
      SDO (Serial Data Output) pin configuration structure.
      
   .. c:member:: pin_config_t ss
   
      SS (Slave Select) pin configuration structure (optional).

UART Pin Configuration Structure
--------------------------------
.. c:type:: uart_pin_config_t

   Structure that holds UART pin configuration details.

   .. c:member:: pin_config_t tx
   
      TX (Transmit) pin configuration structure.
      
   .. c:member:: pin_config_t rx
   
      RX (Receive) pin configuration structure.

PWM Pin Configuration Structure
-------------------------------
.. c:type:: pwm_pin_config_t

   Structure that holds PWM pin configuration details.

   .. c:member:: pin_config_t output
   
      PWM output pin configuration structure.

External Interrupt Pin Configuration Structure
----------------------------------------------
.. c:type:: ext_int_pin_config_t

   Structure that holds external interrupt pin configuration details.

   .. c:member:: pin_config_t input
   
      External interrupt input pin configuration structure.

Functions
=========

Dynamic I2C Configuration
-------------------------
.. c:function:: w_status_t pps_configure_i2c(uint8_t i2c_module, i2c_pin_config_t pin_config)

   :param i2c_module: I2C module number (1 or 2)
   :param pin_config: I2C pin configuration structure
   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   :retval: ``W_INVALID_PARAM`` if module number or pin parameters are invalid

   Configure I2C pins dynamically for the specified module. Sets up both input and output 
   mappings since I2C is bidirectional, configures pins for digital mode and open-drain 
   operation as required for I2C communication.
   
   **Features:**
   
   * Configures both SCL and SDA pins for open-drain operation
   * Sets up bidirectional PPS mappings for I2C communication
   * Validates module number (1-2) and pin parameters
   * Atomic operation with automatic PPS lock management

Dynamic SPI Configuration
-------------------------
.. c:function:: w_status_t pps_configure_spi(uint8_t spi_module, spi_pin_config_t pin_config, bool use_ss)

   :param spi_module: SPI module number (1 or 2)
   :param pin_config: SPI pin configuration structure
   :param use_ss: Whether to configure slave select pin
   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   :retval: ``W_INVALID_PARAM`` if module number or pin parameters are invalid

   Configure SPI pins dynamically for master mode operation. Sets up appropriate pin
   directions (SCK and SDO as outputs, SDI as input) and PPS mappings. Optionally 
   configures slave select pin.
   
   **Features:**
   
   * Configures pins for SPI master mode operation
   * Optional slave select (SS) pin configuration
   * Validates module number (1-2) and pin parameters
   * Proper input/output pin direction setup

Dynamic UART Configuration
--------------------------
.. c:function:: w_status_t pps_configure_uart(uint8_t uart_module, uart_pin_config_t pin_config)

   :param uart_module: UART module number (1-5)
   :param pin_config: UART pin configuration structure
   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   :retval: ``W_INVALID_PARAM`` if module number or pin parameters are invalid

   Configure UART pins dynamically for the specified module. Sets up TX as output and RX 
   as input with appropriate PPS mappings.
   
   **Features:**
   
   * Supports UART modules 1-5
   * Configures TX pin as output, RX pin as input
   * Sets up proper PPS input and output mappings
   * Validates module number and pin parameters

Dynamic PWM Configuration
-------------------------
.. c:function:: w_status_t pps_configure_pwm(uint8_t ccp_module, pwm_pin_config_t pin_config)

   :param ccp_module: CCP module number (1-5)
   :param pin_config: PWM pin configuration structure
   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   :retval: ``W_INVALID_PARAM`` if module number or pin parameters are invalid

   Configure PWM/CCP pins dynamically for the specified module. Sets up the output pin
   direction and PPS mapping. This must be called before initializing the PWM module.
   
   **Features:**
   
   * Supports CCP modules 1-5 for PWM output
   * Configures output pin direction and PPS mapping
   * Must be called before PWM module initialization
   * Validates module number and pin parameters

Dynamic External Interrupt Configuration
----------------------------------------
.. c:function:: w_status_t pps_configure_external_interrupt(uint8_t int_number, ext_int_pin_config_t pin_config)

   :param int_number: External interrupt number (0-2 for INT0, INT1, INT2)
   :param pin_config: External interrupt pin configuration structure
   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   :retval: ``W_INVALID_PARAM`` if interrupt number or pin parameters are invalid

   Configure external interrupt pins dynamically for the specified interrupt. Sets up the 
   pin as a digital input and maps it to the interrupt input via PPS.
   
   **Features:**
   
   * Supports external interrupts INT0, INT1, and INT2
   * Configures pin as digital input
   * Maps pin to interrupt input via PPS
   * Validates interrupt number and pin parameters

Dynamic Timer Clock Configuration
---------------------------------
.. c:function:: w_status_t pps_configure_timer_clk(uint8_t timer, pin_config_t pin_config)

   :param timer: Timer number to configure (0-6)
   :param pin_config: Pin configuration for the external clock input
   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   :retval: ``W_INVALID_PARAM`` if timer number or pin parameters are invalid

   Configure external clock input for the specified timer. Sets up the pin as a digital
   input and maps it to the timer's external clock input via PPS.
   
   **Features:**
   
   * Supports Timer0 through Timer6
   * Configures pin as digital input for external clock
   * Maps pin to timer clock input via PPS
   * Validates timer number and pin parameters

Dynamic Timer Gate Configuration
--------------------------------
.. c:function:: w_status_t pps_configure_timer_gate(uint8_t timer, pin_config_t pin_config)

   :param timer: Timer number to configure (0-6)
   :param pin_config: Pin configuration for the timer gate input
   :returns: Status code indicating success or failure
   :retval: ``W_SUCCESS`` if configuration was successful
   :retval: ``W_INVALID_PARAM`` if timer number or pin parameters are invalid

   Configure timer gate input for the specified timer. Sets up the pin as a digital
   input and maps it to the timer's gate input via PPS.
   
   **Features:**
   
   * Supports Timer0 through Timer6
   * Configures pin as digital input for gate control
   * Maps pin to timer gate input via PPS
   * Validates timer number and pin parameters

Security and Safety Features
============================

PPS Lock Management
-------------------

The module implements robust PPS register lock management to ensure system security:

* **Automatic Lock/Unlock**: All configuration functions automatically handle PPS register unlocking and locking
* **Interrupt Safety**: PPS operations preserve the current interrupt state during critical sequences
* **Error Recovery**: If any configuration step fails, PPS registers are always locked before returning
* **Cleanup Pattern**: All functions use a consistent cleanup pattern with ``goto`` statements to ensure proper resource management

Lock/Unlock Sequence
--------------------

The PPS lock/unlock mechanism follows these steps:

1. **Save Interrupt State**: Current GIE (Global Interrupt Enable) state is preserved
2. **Disable Interrupts**: Interrupts are temporarily disabled during the critical sequence
3. **Unlock Sequence**: Write 0x55, then 0xAA to PPSLOCK register, then clear PPSLOCKED bit
4. **Configuration**: Perform the required pin and PPS configuration
5. **Lock Sequence**: Write 0x55, then 0xAA to PPSLOCK register, then set PPSLOCKED bit
6. **Restore Interrupts**: Original GIE state is restored

**Security Benefits:**

* Prevents accidental PPS register modification
* Ensures atomic configuration operations
* Maintains interrupt context integrity
* Provides predictable behavior in all execution contexts

Usage Examples
==============

Basic Usage with Multiple Modules
----------------------------------

Example showing configuration of multiple peripheral modules:

.. code-block:: c

   int main(void) {
       // Initialize the microcontroller (clocks, etc.)
       mcu_init();
       
       // Configure I2C1 on RC3/RC4
       i2c_pin_config_t i2c1_config = {
           .scl = {.port = 2, .pin = 3}, // RC3
           .sda = {.port = 2, .pin = 4}  // RC4
       };
       w_status_t status = pps_configure_i2c(1, i2c1_config);
       if (status != W_SUCCESS) {
           // Handle error
           return -1;
       }
       
       // Configure I2C2 on different pins: RA1/RA2
       i2c_pin_config_t i2c2_config = {
           .scl = {.port = 0, .pin = 1}, // RA1
           .sda = {.port = 0, .pin = 2}  // RA2
       };
       status = pps_configure_i2c(2, i2c2_config);
       if (status != W_SUCCESS) {
           // Handle error
           return -1;
       }
       
       // Configure PWM on CCP1 using RB5
       pwm_pin_config_t pwm_config = {
           .output = {.port = 1, .pin = 5} // RB5
       };
       status = pps_configure_pwm(1, pwm_config);
       if (status != W_SUCCESS) {
           // Handle error
           return -1;
       }
       
       // Configure UART1 on RC6/RC7
       uart_pin_config_t uart_config = {
           .tx = {.port = 2, .pin = 6}, // RC6
           .rx = {.port = 2, .pin = 7}  // RC7
       };
       status = pps_configure_uart(1, uart_config);
       if (status != W_SUCCESS) {
           // Handle error
           return -1;
       }
       
       // Configure external interrupt INT0 on RB0
       ext_int_pin_config_t int_config = {
           .input = {.port = 1, .pin = 0} // RB0
       };
       status = pps_configure_external_interrupt(0, int_config);
       if (status != W_SUCCESS) {
           // Handle error
           return -1;
       }
       
       // Now initialize peripherals (pin configuration is done)
       i2c_init(frequency_divider);
       pwm_init(1, pwm_period);  // No pin config needed, already done
       uart_init(baud_rate);
       
       while(1) {
           // Main application loop
       }
   }

Advanced SPI Configuration with Slave Select
--------------------------------------------

Example showing SPI configuration with optional slave select:

.. code-block:: c

   // Configure SPI1 with slave select for external device control
   spi_pin_config_t spi_config = {
       .sck = {.port = 2, .pin = 0},  // RC0 - Serial Clock
       .sdi = {.port = 2, .pin = 1},  // RC1 - Serial Data Input
       .sdo = {.port = 2, .pin = 2},  // RC2 - Serial Data Output
       .ss = {.port = 2, .pin = 5}    // RC5 - Slave Select
   };
   
   // Configure with slave select enabled
   w_status_t status = pps_configure_spi(1, spi_config, true);
   if (status != W_SUCCESS) {
       // Handle configuration error
       return -1;
   }
   
   // Initialize SPI peripheral after pin configuration
   spi_init();

Modular Design Example
----------------------

Example showing separation of concerns:

.. code-block:: c

   // In pin_setup.c - Centralized pin configuration
   w_status_t setup_all_pins(void) {
       w_status_t status;
       
       // Configure all I2C pins
       i2c_pin_config_t i2c_config = {
           .scl = {.port = 2, .pin = 3},
           .sda = {.port = 2, .pin = 4}
       };
       status = pps_configure_i2c(1, i2c_config);
       if (status != W_SUCCESS) return status;
       
       // Configure all PWM pins
       pwm_pin_config_t pwm1_config = {.output = {.port = 1, .pin = 5}};
       status = pps_configure_pwm(1, pwm1_config);
       if (status != W_SUCCESS) return status;
       
       pwm_pin_config_t pwm2_config = {.output = {.port = 1, .pin = 6}};
       status = pps_configure_pwm(2, pwm2_config);
       if (status != W_SUCCESS) return status;
       
       // Configure SPI with slave select
       spi_pin_config_t spi_config = {
           .sck = {.port = 2, .pin = 0},
           .sdi = {.port = 2, .pin = 1},
           .sdo = {.port = 2, .pin = 2},
           .ss = {.port = 2, .pin = 5}
       };
       status = pps_configure_spi(1, spi_config, true);
       if (status != W_SUCCESS) return status;
       
       // Configure timer external clock
       pin_config_t timer_clk = {.port = 1, .pin = 3};
       status = pps_configure_timer_clk(1, timer_clk);
       if (status != W_SUCCESS) return status;
       
       return W_SUCCESS;
   }
   
   // In main.c - Clean peripheral initialization
   int main(void) {
       mcu_init();
       
       // Configure ALL pins first
       if (setup_all_pins() != W_SUCCESS) {
           // Handle error
           return -1;
       }
       
       // Then initialize peripherals (no pin concerns)
       i2c_init(400000);  // 400kHz I2C
       pwm_init(1, 1000); // PWM1 with 1000 period
       pwm_init(2, 2000); // PWM2 with 2000 period
       spi_init();        // SPI master mode
       
       while(1) {
           // Application logic
       }
   }

Timer Configuration Example
---------------------------

Example showing timer external clock and gate configuration:

.. code-block:: c

   // Configure Timer1 external clock on RB3
   pin_config_t timer1_clk = {.port = 1, .pin = 3}; // RB3
   w_status_t status = pps_configure_timer_clk(1, timer1_clk);
   if (status != W_SUCCESS) {
       // Handle error
       return -1;
   }
   
   // Configure Timer1 gate control on RB4
   pin_config_t timer1_gate = {.port = 1, .pin = 4}; // RB4
   status = pps_configure_timer_gate(1, timer1_gate);
   if (status != W_SUCCESS) {
       // Handle error
       return -1;
   }
   
   // Now initialize timer with external clock and gate
   timer1_init_external_clock();

Implementation Details
======================

The PPS configuration module uses helper functions following BARR-C coding standards:

Module Support
--------------

* **I2C**: Supports I2C1 and I2C2 modules with automatic open-drain configuration
* **SPI**: Supports SPI1 and SPI2 modules with optional slave select for master mode
* **UART**: Supports UART1 through UART5 modules with proper TX/RX configuration
* **PWM/CCP**: Supports CCP1 through CCP5 modules for PWM output
* **External Interrupts**: Supports INT0, INT1, and INT2 with proper input configuration
* **Timer**: Supports Timer0 through Timer6 external clock and gate inputs

Helper Functions
----------------

All register access is done through proper functions (no function-like macros per BARR-C):

**Register Access Functions:**
* ``get_tris_register()`` - Get TRIS register for port direction control
* ``get_ansel_register()`` - Get ANSEL register for analog/digital mode selection
* ``get_odcon_register()`` - Get ODCON register for open-drain configuration
* ``get_pps_output_register()`` - Get PPS output register for port/pin mapping

**Configuration Functions:**
* ``configure_pin_digital()`` - Configure pin for digital I/O with direction setting
* ``configure_pin_open_drain()`` - Configure pin for open-drain mode (I2C)
* ``configure_pps_output()`` - Configure PPS output mapping for peripherals
* ``validate_pin_config()`` - Validate pin configuration parameters

**PPS Management Functions:**
* ``pps_unlock()`` - Safely unlock PPS registers with interrupt state preservation
* ``pps_lock()`` - Safely lock PPS registers with interrupt state restoration

PPS Codes
---------

The module defines peripheral codes for PPS configuration:

**I2C Output Codes:**
* ``PPS_I2C1_SCL_OUTPUT``: 0b100001 - I2C1 Serial Clock output
* ``PPS_I2C1_SDA_OUTPUT``: 0b100010 - I2C1 Serial Data output
* ``PPS_I2C2_SCL_OUTPUT``: 0b100011 - I2C2 Serial Clock output
* ``PPS_I2C2_SDA_OUTPUT``: 0b100100 - I2C2 Serial Data output

**SPI Output Codes:**
* ``PPS_SPI1_SCK_OUTPUT``: 0b011110 - SPI1 Serial Clock output
* ``PPS_SPI1_SDO_OUTPUT``: 0b011111 - SPI1 Serial Data Output
* ``PPS_SPI2_SCK_OUTPUT``: 0b100000 - SPI2 Serial Clock output
* ``PPS_SPI2_SDO_OUTPUT``: 0b100001 - SPI2 Serial Data Output

**UART Output Codes:**
* ``PPS_UART1_TX_OUTPUT``: 0b010011 - UART1 Transmit output
* ``PPS_UART2_TX_OUTPUT``: 0b010100 - UART2 Transmit output
* ``PPS_UART3_TX_OUTPUT``: 0b010101 - UART3 Transmit output
* ``PPS_UART4_TX_OUTPUT``: 0b010110 - UART4 Transmit output
* ``PPS_UART5_TX_OUTPUT``: 0b010111 - UART5 Transmit output

**PWM/CCP Output Codes:**
* ``PPS_CCP1_OUTPUT``: 0b001100 - CCP1/PWM1 output
* ``PPS_CCP2_OUTPUT``: 0b001101 - CCP2/PWM2 output
* ``PPS_CCP3_OUTPUT``: 0b001110 - CCP3/PWM3 output
* ``PPS_CCP4_OUTPUT``: 0b001111 - CCP4/PWM4 output
* ``PPS_CCP5_OUTPUT``: 0b010000 - CCP5/PWM5 output

Error Handling and Recovery
---------------------------

The implementation provides comprehensive error handling:

**Parameter Validation:**
* Module number range checking (e.g., I2C: 1-2, UART: 1-5, CCP: 1-5)
* Port number validation (0-2 for PORTA-PORTC)
* Pin number validation (0-7 for each port)

**Error Recovery:**
* Cleanup pattern ensures PPS registers are always locked
* All functions use ``goto cleanup`` for error paths after PPS unlock
* Original interrupt state is always restored
* Failed operations leave system in consistent state

**Error Codes:**
* ``W_SUCCESS`` - Operation completed successfully
* ``W_INVALID_PARAM`` - Invalid module number, port, or pin parameter

Architecture and Design Principles
==================================

Separation of Concerns
----------------------

The pin configuration module follows strict separation of concerns:

* **Pin Configuration Layer**: Handles all pin-related setup (direction, mode, PPS mapping)
* **Peripheral Layers**: Focus solely on peripheral functionality and register configuration
* **Application Layer**: Uses both layers independently with clear interfaces

This design provides several benefits:

* **Modularity**: Each module has a single, well-defined responsibility
* **Testability**: Pin configuration can be tested independently of peripheral logic
* **Flexibility**: Pin mappings can be changed without modifying peripheral code
* **Maintainability**: Clear boundaries make code easier to understand and modify


Thread Safety and Interrupt Handling
------------------------------------

The module provides robust interrupt safety:

* **Interrupt State Preservation**: Current GIE state is saved and restored
* **Atomic Operations**: PPS configuration is atomic with respect to interrupts
* **Critical Section Management**: Minimal time spent with interrupts disabled
* **Predictable Behavior**: Safe to call from interrupt context or initialization code

Best Practices
==============

Configuration Order
-------------------

Follow this recommended order for system initialization:

1. **MCU Initialization**: Set up clocks, power management
2. **Pin Configuration**: Configure all peripheral pins using this module
3. **Peripheral Initialization**: Initialize peripheral modules
4. **Application Logic**: Start main application functionality

Notes
=====

* **Separation of Concerns**: Pin configuration is handled entirely by this module
* **Module Independence**: PWM, I2C, SPI, UART modules focus only on their functionality
* **Multiple Module Support**: Supports multiple instances of peripherals where available
* **Parameter Validation**: All functions validate module numbers and pin parameters
* **Error Handling**: Returns appropriate error codes for invalid configurations
* **Security**: Robust PPS lock management prevents accidental register modification
* **Interrupt Safety**: Preserves interrupt state during critical PPS sequences
* **Resource Management**: Automatic cleanup ensures system consistency