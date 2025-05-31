I2C Master Driver
******************

The I2C driver provides master-only communication functionality for the PIC18F26K83. The driver focuses purely on I2C protocol implementation, while pin configuration is handled separately by the dedicated pin configuration module.

**Key Features:**

* Master-only operation (slave mode not supported)
* Support for I2C1 and I2C2 modules
* 8-bit and 16-bit register access functions
* 7-bit addressing support
* Separation of concerns: pin configuration handled separately

**Current Status:** Under development

Prerequisites
=============

Before using the I2C driver, you must configure the I2C pins using the pin configuration module:

.. code-block:: c

   // Configure I2C1 pins first
   i2c_pin_config_t i2c_config = {
       .scl = {.port = 2, .pin = 3}, // RC3 for SCL
       .sda = {.port = 2, .pin = 4}  // RC4 for SDA
   };
   w_status_t status = pps_configure_i2c(1, i2c_config);
   if (status != W_SUCCESS) {
       // Handle configuration error
       return -1;
   }
   
   // Then initialize I2C module
   i2c_init(freq_divider);

For details on I2C pin configuration, see the :doc:`pin_config` documentation.

Design Assumptions
==================

The current I2C driver implementation is based on these assumptions:

* **Master Mode Only**: The MCU operates as I2C master since slave operation is unlikely in rocket applications
* **7-bit Addressing**: I2C device addresses are 7 bits (excluding R/W bit)
* **Single Controller**: Only one I2C controller is used at a time (though hardware supports I2C1 and I2C2)
* **8-bit Register Addresses**: Device register addresses are limited to 8 bits

I2C Controller Functions
========================

Initialization
--------------
.. c:function:: void i2c_init(uint8_t freq_div)

   Initialize I2C module for master mode operation.

   :param freq_div: Frequency divider for I2C clock. I2C Frequency = 100 kHz / freq_div
   :type freq_div: uint8_t
   
   .. note::
      Pin configuration must be completed using :c:func:`pps_configure_i2c` before calling this function.

Data Transfer Functions
-----------------------

.. c:function:: bool i2c_write_data(uint8_t i2c_addr, const uint8_t *data, uint8_t len)

   Send data through I2C. Typically used for sending register addresses and write data.

   :param i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the R/W bit
   :type i2c_addr: uint8_t
   :param data: Data to be transmitted
   :type data: const uint8_t*
   :param len: Length of data to be transmitted in bytes (I2C address byte not included)
   :type len: uint8_t
   :returns: Transfer status
   :retval true: Success
   :retval false: Failed
   :rtype: bool

.. c:function:: bool i2c_read_data(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint8_t len)

   Receive data through I2C. Typically used for reading register data.

   :param i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the R/W bit
   :type i2c_addr: uint8_t
   :param reg_addr: Device register address (8 bits max)
   :type reg_addr: uint8_t
   :param data: Buffer for received data
   :type data: uint8_t*
   :param len: Length of data to be received in bytes
   :type len: uint8_t
   :returns: Transfer status
   :retval true: Success
   :retval false: Failed
   :rtype: bool

Register Access Functions
-------------------------

8-bit Register Functions
~~~~~~~~~~~~~~~~~~~~~~~~

.. c:function:: bool i2c_read_reg8(uint8_t i2c_addr, uint8_t reg_addr, uint8_t* value)

   Read a byte-wide device register.

   :param i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the R/W bit
   :type i2c_addr: uint8_t
   :param reg_addr: Device register address (8 bits max)
   :type reg_addr: uint8_t
   :param value: Pointer to register value buffer
   :type value: uint8_t*
   :returns: Operation status
   :retval true: Success
   :retval false: Failed
   :rtype: bool

.. c:function:: bool i2c_write_reg8(uint8_t i2c_addr, uint8_t reg_addr, uint8_t value)

   Write a byte-wide device register.

   :param i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the R/W bit
   :type i2c_addr: uint8_t
   :param reg_addr: Device register address (8 bits max)
   :type reg_addr: uint8_t
   :param value: Value to be written to the register
   :type value: uint8_t
   :returns: Operation status
   :retval true: Success
   :retval false: Failed
   :rtype: bool

16-bit Register Functions
~~~~~~~~~~~~~~~~~~~~~~~~~

.. c:function:: bool i2c_read_reg16(uint8_t i2c_addr, uint8_t reg_addr, uint16_t* value)

   Read a 2-byte device register.

   :param i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the R/W bit
   :type i2c_addr: uint8_t
   :param reg_addr: Device register address (8 bits max)
   :type reg_addr: uint8_t
   :param value: Pointer to register value buffer
   :type value: uint16_t*
   :returns: Operation status
   :retval true: Success
   :retval false: Failed
   :rtype: bool

.. c:function:: bool i2c_write_reg16(uint8_t i2c_addr, uint8_t reg_addr, uint16_t value)

   Write a 2-byte device register.

   :param i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the R/W bit
   :type i2c_addr: uint8_t
   :param reg_addr: Device register address (8 bits max)
   :type reg_addr: uint8_t
   :param value: Value to be written to the register
   :type value: uint16_t
   :returns: Operation status
   :retval true: Success
   :retval false: Failed
   :rtype: bool

Usage Example
=============

Complete I2C Setup and Usage
----------------------------

.. code-block:: c

   #include "pin_config.h"
   #include "i2c.h"
   
   int main(void) {
       // Initialize MCU
       mcu_init();
       
       // Configure I2C1 pins
       i2c_pin_config_t i2c_config = {
           .scl = {.port = 2, .pin = 3}, // RC3
           .sda = {.port = 2, .pin = 4}  // RC4
       };
       
       w_status_t status = pps_configure_i2c(1, i2c_config);
       if (status != W_SUCCESS) {
           // Handle pin configuration error
           return -1;
       }
       
       // Initialize I2C at 400kHz (100kHz / 0.25)
       i2c_init(1); // freq_div = 1 for ~100kHz, adjust as needed
       
       // Example: Read from a sensor
       uint8_t sensor_addr = 0x48; // 7-bit address (will be shifted for R/W)
       uint8_t reg_addr = 0x00;    // Temperature register
       uint16_t temperature;
       
       if (i2c_read_reg16(sensor_addr, reg_addr, &temperature)) {
           // Successfully read temperature
           // Process temperature data
       } else {
           // Handle I2C communication error
       }
       
       while(1) {
           // Main application loop
       }
   }

Device Communication Pattern
-----------------------------

.. code-block:: c

   // Example: Configure a device register
   uint8_t device_addr = 0x1D;  // Accelerometer address
   uint8_t config_reg = 0x20;   // Control register
   uint8_t config_val = 0x47;   // Enable XYZ axes, 50Hz
   
   if (!i2c_write_reg8(device_addr, config_reg, config_val)) {
       // Handle write error
   }
   
   // Read acceleration data
   uint8_t accel_data[6];
   if (i2c_read_data(device_addr, 0x28, accel_data, 6)) {
       // Process XYZ acceleration data
       int16_t accel_x = (accel_data[1] << 8) | accel_data[0];
       int16_t accel_y = (accel_data[3] << 8) | accel_data[2];
       int16_t accel_z = (accel_data[5] << 8) | accel_data[4];
   }

Architecture Notes
==================

**Separation of Concerns:**
* Pin configuration is handled by the pin configuration module
* I2C driver focuses solely on protocol implementation
* Clean interfaces between modules enable independent testing and development

**Multi-Module Support:**
While the current implementation assumes single controller usage, the hardware supports both I2C1 and I2C2. Future enhancements could extend the API to support multiple simultaneous I2C controllers.

**Error Handling:**
Functions return boolean status codes. Consider implementing more detailed error reporting in future revisions for better debugging and fault diagnosis.

Implementation Status
======================

**Current Status:** Under development

**Limitations:**
* Single I2C controller support (hardware supports I2C1 and I2C2)
* 8-bit register address limitation
* Boolean-only error reporting

**Future Enhancements:**
* Multi-controller support
* Extended register address support (16-bit)
* Detailed error codes
* Interrupt-driven operation
* DMA support for bulk transfers
