I2C Master Driver
*****************

The driver handles I2C master mode operations with automatic bus recovery for collision detection. The driver includes:

- Automatic bus collision recovery (9-clock pulse generation + 35ms SCL hold)
- Repeated START support for register read operations
- Timeout protection for all I2C operations
- Functions for reading and writing 8 and 16 bit registers

Assumptions in this revision
============================
- I2C Address(exclude R/W bits) are 7 bits
- Only one I2C controller could be used
- Register address are 8 bit wide
- Automatic STOP generation when I2CCNT reaches 0 (PIC18F26K83 standalone I2C module)

I2C Controller Functions
========================

.. c:function:: w_status_t i2c_init(uint8_t clkdiv)

   Initialize I2C module with clock divider configuration

   :param uint8_t clkdiv: clock divider (0-7) for I2C frequency selection
   :return: W_SUCCESS on success, W_IO_ERROR or W_IO_TIMEOUT on failure

.. c:function:: w_status_t i2c_write_data(uint8_t address, const uint8_t *data, uint8_t len)

   Send data through I2C (raw write operation)

   :param uint8_t address: 7-bit I2C peripheral address (without R/W bit)
   :param const uint8_t* data: data to be transmitted
   :param uint8_t len: length of data to be transmitted in bytes
   :return: W_SUCCESS on success, W_IO_ERROR or W_IO_TIMEOUT on failure

.. c:function:: w_status_t i2c_read_data(uint8_t address, uint8_t *data, uint8_t len)

   Receive data through I2C (raw read operation)

   :param uint8_t address: 7-bit I2C peripheral address (without R/W bit)
   :param uint8_t* data: buffer for received data
   :param uint8_t len: length of data to be received in bytes
   :return: W_SUCCESS on success, W_IO_ERROR or W_IO_TIMEOUT on failure

.. c:function:: w_status_t i2c_read_reg8(uint8_t address, uint8_t reg, uint8_t* value)

   Read a byte-wide device register (uses repeated START)

   :param uint8_t address: 7-bit I2C peripheral address (without R/W bit)
   :param uint8_t reg: device register address
   :param uint8_t* value: pointer to register value buffer
   :return: W_SUCCESS on success, W_IO_ERROR or W_IO_TIMEOUT on failure

.. c:function:: w_status_t i2c_read_reg16(uint8_t address, uint8_t reg, uint16_t* value)

   Read a 2-byte device register (uses repeated START, MSB first)

   :param uint8_t address: 7-bit I2C peripheral address (without R/W bit)
   :param uint8_t reg: device register address
   :param uint16_t* value: pointer to register value buffer
   :return: W_SUCCESS on success, W_IO_ERROR or W_IO_TIMEOUT on failure

.. c:function:: w_status_t i2c_write_reg8(uint8_t address, uint8_t reg, uint8_t val)

   Write a byte-wide device register

   :param uint8_t address: 7-bit I2C peripheral address (without R/W bit)
   :param uint8_t reg: device register address
   :param uint8_t val: value to be written to the register
   :return: W_SUCCESS on success, W_IO_ERROR or W_IO_TIMEOUT on failure

.. c:function:: w_status_t i2c_write_reg16(uint8_t address, uint8_t reg, uint16_t val)

   Write a 2-byte device register (MSB first)

   :param uint8_t address: 7-bit I2C peripheral address (without R/W bit)
   :param uint8_t reg: device register address
   :param uint16_t val: value to be written to the register
   :return: W_SUCCESS on success, W_IO_ERROR or W_IO_TIMEOUT on failure
