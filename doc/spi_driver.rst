SPI Driver
**********

The SPI driver provides master mode communication functionality for the PIC18F26K83. The driver focuses on SPI protocol implementation, while pin configuration is handled separately by the dedicated pin configuration module.

**Key Features:**

* Master mode operation (slave mode not currently supported)
* Support for SPI1 and SPI2 modules  
* 8-bit register read/write functions for peripheral devices
* Chip select signal management through GPIO integration
* Separation of concerns: pin configuration handled separately

**Current Status:** Interface defined, implementation pending

Prerequisites
=============

Before using the SPI driver, you must configure the SPI pins using the pin configuration module:

.. code-block:: c

   // Configure SPI1 pins first
   spi_pin_config_t spi_config = {
       .sck = {.port = 2, .pin = 0}, // RC0 for Serial Clock
       .sdi = {.port = 2, .pin = 1}, // RC1 for Serial Data Input
       .sdo = {.port = 2, .pin = 2}, // RC2 for Serial Data Output
       .ss = {.port = 2, .pin = 5}   // RC5 for Slave Select (optional)
   };
   
   // Configure with slave select enabled
   w_status_t status = pps_configure_spi(1, spi_config, true);
   if (status != W_SUCCESS) {
       // Handle configuration error
       return -1;
   }
   
   // Then initialize SPI module
   spi_init(freq_divider);

For details on SPI pin configuration, see the :doc:`pin_config` documentation.

Design Features
===============

**Master Mode Operation:**
The driver is designed for master mode operation where the MCU controls the clock and initiates all communications.

**Chip Select Management:**
Chip select (CS) signals are managed through GPIO driver integration, allowing flexible control of multiple SPI devices.

**Register-based Communication:**
Optimized functions for common peripheral register operations (read/write 8-bit registers).

SPI Controller Functions
========================

Initialization
--------------
.. c:function:: void spi_init(uint8_t freq)

   Initialize SPI module for master mode operation.

   :param freq: Frequency divider for SPI clock generation
   :type freq: uint8_t
   
   .. note::
      Pin configuration must be completed using :c:func:`pps_configure_spi` before calling this function.
      
   .. note::
      Exact frequency calculation is TBD and will be documented when implementation is complete.

Low-Level Transfer Functions
-----------------------------

.. c:function:: void spi_tx(uint8_t data)

   Transmit a single byte via SPI.

   :param data: 8-bit data to transmit
   :type data: uint8_t
   
   This function initiates an SPI transmission. For full-duplex operation, combine with :c:func:`spi_rx`.

.. c:function:: uint8_t spi_rx(void)

   Receive a single byte from SPI.

   :returns: Received data byte
   :rtype: uint8_t
   
   This function reads the SPI receive buffer. In master mode, a transmission must be initiated first to generate the clock for reception.

High-Level Register Functions
-----------------------------

These functions are designed for easy peripheral device register access:

.. c:function:: void spi_write8(uint8_t reg_addr, uint8_t value)

   Write to an 8-bit register of an SPI peripheral device.

   :param reg_addr: 8-bit register address
   :type reg_addr: uint8_t
   :param value: 8-bit data to be written
   :type value: uint8_t
   
   .. important::
      Chip select (CS) line must be toggled manually before and after calling this function.

.. c:function:: uint8_t spi_read8(uint8_t reg_addr)

   Read from an 8-bit register of an SPI peripheral device.

   :param reg_addr: 8-bit register address to read from
   :type reg_addr: uint8_t
   :returns: Data read from the register
   :rtype: uint8_t
   
   .. important::
      Chip select (CS) line must be toggled manually before and after calling this function.

Usage Examples
==============

Basic SPI Setup and Communication
---------------------------------

.. code-block:: c

   #include "pin_config.h"
   #include "spi.h"
   #include "gpio.h"
   
   int main(void) {
       // Initialize MCU
       mcu_init();
       
       // Configure SPI1 pins
       spi_pin_config_t spi_config = {
           .sck = {.port = 2, .pin = 0}, // RC0
           .sdi = {.port = 2, .pin = 1}, // RC1
           .sdo = {.port = 2, .pin = 2}, // RC2
           .ss = {.port = 2, .pin = 5}   // RC5
       };
       
       w_status_t status = pps_configure_spi(1, spi_config, true);
       if (status != W_SUCCESS) {
           // Handle pin configuration error
           return -1;
       }
       
       // Configure CS pin as GPIO output (for manual control)
       gpio_enable_output(C, 5); // RC5 as CS
       gpio_set_output(C, 5, 1);  // CS idle high
       
       // Initialize SPI module
       spi_init(4); // Example frequency divider
       
       while(1) {
           // Example: Read device ID from SPI device
           gpio_set_output(C, 5, 0); // Assert CS
           uint8_t device_id = spi_read8(0x00); // Read ID register
           gpio_set_output(C, 5, 1); // Deassert CS
           
           // Process device_id...
           
           delay_ms(1000);
       }
   }

Multiple SPI Device Management
-------------------------------

.. code-block:: c

   #include "pin_config.h"
   #include "spi.h"
   #include "gpio.h"
   
   // Define chip select pins for different devices
   #define FLASH_CS_PORT  C
   #define FLASH_CS_PIN   5
   #define SENSOR_CS_PORT C
   #define SENSOR_CS_PIN  6
   
   // Helper functions for chip select management
   void flash_cs_assert(void) {
       gpio_set_output(FLASH_CS_PORT, FLASH_CS_PIN, 0);
   }
   
   void flash_cs_deassert(void) {
       gpio_set_output(FLASH_CS_PORT, FLASH_CS_PIN, 1);
   }
   
   void sensor_cs_assert(void) {
       gpio_set_output(SENSOR_CS_PORT, SENSOR_CS_PIN, 0);
   }
   
   void sensor_cs_deassert(void) {
       gpio_set_output(SENSOR_CS_PORT, SENSOR_CS_PIN, 1);
   }
   
   int main(void) {
       mcu_init();
       
       // Configure SPI pins (shared by both devices)
       spi_pin_config_t spi_config = {
           .sck = {.port = 2, .pin = 0}, // RC0
           .sdi = {.port = 2, .pin = 1}, // RC1  
           .sdo = {.port = 2, .pin = 2}, // RC2
           // Note: .ss not used for manual CS control
       };
       pps_configure_spi(1, spi_config, false); // No automatic SS
       
       // Configure individual CS pins
       gpio_enable_output(FLASH_CS_PORT, FLASH_CS_PIN);
       gpio_enable_output(SENSOR_CS_PORT, SENSOR_CS_PIN);
       flash_cs_deassert();  // CS idle high
       sensor_cs_deassert(); // CS idle high
       
       spi_init(4);
       
       while(1) {
           // Communicate with flash memory
           flash_cs_assert();
           spi_write8(0x06, 0x00); // Write enable command
           flash_cs_deassert();
           
           delay_ms(10);
           
           // Communicate with sensor
           sensor_cs_assert();
           uint8_t temp_data = spi_read8(0x00); // Read temperature
           sensor_cs_deassert();
           
           // Process data...
           delay_ms(1000);
       }
   }

Advanced Register Operations
-----------------------------

.. code-block:: c

   // Example: Configure an accelerometer via SPI
   void accelerometer_init(void) {
       // Assert CS
       gpio_set_output(ACCEL_CS_PORT, ACCEL_CS_PIN, 0);
       
       // Configure control register 1: 50Hz, XYZ enable
       spi_write8(0x20, 0x47);
       
       // Configure control register 4: +/-2g scale
       spi_write8(0x23, 0x00);
       
       // Deassert CS
       gpio_set_output(ACCEL_CS_PORT, ACCEL_CS_PIN, 1);
   }
   
   void read_acceleration(int16_t *x, int16_t *y, int16_t *z) {
       uint8_t accel_data[6];
       
       gpio_set_output(ACCEL_CS_PORT, ACCEL_CS_PIN, 0);
       
       // Read 6 bytes starting from X_L register
       for (uint8_t i = 0; i < 6; i++) {
           accel_data[i] = spi_read8(0x28 + i);
       }
       
       gpio_set_output(ACCEL_CS_PORT, ACCEL_CS_PIN, 1);
       
       // Combine low and high bytes
       *x = (int16_t)((accel_data[1] << 8) | accel_data[0]);
       *y = (int16_t)((accel_data[3] << 8) | accel_data[2]);
       *z = (int16_t)((accel_data[5] << 8) | accel_data[4]);
   }

Architecture Integration
=========================

SPI Bus Sharing
---------------

The SPI driver is designed to support multiple devices on the same SPI bus:

.. code-block:: text

   MCU (Master)
        |
        +----- SCK ----+---- Device 1 (SCK)
        |              |
        +----- SDI ----+---- Device 1 (SDO)
        |              |
        +----- SDO ----+---- Device 1 (SDI)
        |              |
        +----- CS1 ---------- Device 1 (CS)
        |              |
        +----- CS2 ----+----- Device 2 (CS)
                       |
                       +----- Device 2 (SCK)
                       |
                       +----- Device 2 (SDI)
                       |
                       +----- Device 2 (SDO)

**Key Points:**
* SCK, SDI, SDO are shared among all devices
* Each device has its own CS signal
* Only one device should be active (CS asserted) at a time

Layer Responsibilities
-----------------------

.. code-block:: text

   Application Layer
         |
   +-----v-----+
   | SPI       |  <- High-level register operations
   | Driver    |     Low-level transfer functions
   +-----------+
         |
   +-----v-----+
   | GPIO      |  <- Chip select management
   | Driver    |
   +-----------+
         |
   +-----v-----+
   | Pin       |  <- SPI pin configuration and PPS routing
   | Config    |
   +-----------+
         |
   +-----v-----+
   | Hardware  |  <- SPI peripheral registers
   | SPI       |
   +-----------+

**Integration Benefits:**
* **Modular Design**: Each layer has specific responsibilities
* **Reusability**: GPIO and pin config layers used by other peripherals
* **Testability**: Each layer can be tested independently
* **Flexibility**: Easy to add new SPI devices or change pin assignments

Implementation Status
======================

**Current Status:** Interface defined, implementation pending

**Planned Features:**
* Full-duplex and half-duplex communication modes
* Configurable clock polarity and phase (CPOL/CPHA)
* Multiple frequency options
* Interrupt-driven operation for improved performance

**Design Considerations:**
* **Clock Configuration**: SPI clock will be derived from system clock with configurable dividers
* **Buffer Management**: Transmit and receive operations will be optimized for common use cases
* **Error Handling**: Status reporting for communication errors and timeouts

**Integration with Other Modules:**
* **GPIO Driver**: Used for chip select signal management
* **Pin Configuration**: Handles SPI pin routing and electrical configuration
* **Timer Module**: May be used for timeout functionality

**Future Enhancements:**
* DMA support for high-speed transfers
* Multi-master mode support
* Hardware-controlled chip select for single-device scenarios
* Automatic protocol handling for common device types (EEPROM, ADC, etc.)
