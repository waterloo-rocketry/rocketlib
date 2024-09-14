I2C Master Driver
*****************

The driver only handles when the device is used as an I2C master, because it's unlikely the MCU is going to used as an I2C slave on the rocket. Functions for reading 8 and 16 bit registers are provided.

Assumptions in this revision
============================
- I2C Address(exclude R/W bits) are 7 bits
- Only one I2C controller could be used
- Register address are 8 bit wide.

I2C Controller Functions
========================

.. c:function:: void i2c_init(uint8_t freq_div)

   Initialize I2C module and set up pins

   :param uint8_t freq_div: frequency divider, I2C Frequency = 100 kHz / freq_div

.. c:function:: bool i2c_write_data(uint8_t i2c_addr, const uint8_t *data, uint8_t len)

   Send data through I2C, this is usually used for sending register address and write data.

   :param uint8_t i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the RW bit
   :param const uint8_t* data: data to be transmitted
   :param uint8_t len: length of data to be transmitted in bytes, the I2C address byte is not included
   :return: success or not
   :retval true: success
   :retval false: failed

.. c:function:: bool i2c_read_data(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint8_t len)

   Receive data through I2C, this is usually used for receiving read data.

   :param uint8_t i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the RW bit
   :param uint8_t reg_addr: device register map address, max 8 bits wide
   :param uint8_t* data: buffer for received data
   :param uint8_t len: length of data to be received in bytes
   :return: success or not
   :retval true: success
   :retval false: failed

.. c:function:: bool i2c_read_reg8(uint8_t i2c_addr, uint8_t reg_addr, uint8_t* value)

   Read a byte-wide device register

   :param uint8_t i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the RW bit
   :param uint8_t reg_addr: device register map address, max 8 bits wide
   :param uint8_t* value: pointer to register value buffer
   :return: success or not
   :retval true: success
   :retval false: failed

.. c:function:: bool i2c_read_reg16(uint8_t i2c_addr, uint8_t reg_addr, uint16_t* value)

   Read a 2-byte device register

   :param uint8_t i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the RW bit
   :param uint8_t reg_addr: device register map address, max 8 bits wide
   :param uint16_t* value: pointer to register value buffer
   :return: success or not
   :retval true: success
   :retval false: failed

.. c:function:: bool i2c_write_reg8(uint8_t i2c_addr, uint8_t reg_addr, uint8_t value)

   Write a byte-wide device register

   :param uint8_t i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the RW bit
   :param uint8_t reg_addr: device register map address, max 8 bits wide
   :param uint8_t value: value to be written to the register
   :return: success or not
   :retval true: success
   :retval false: failed

.. c:function:: bool i2c_write_reg16(uint8_t i2c_addr, uint8_t reg_addr, uint16_t value)

   Write a 2-byte device register

   :param uint8_t i2c_addr: I2C peripheral address, where addr[7:1] is the 7-bit address, and addr[0] is the RW bit
   :param uint8_t reg_addr: device register map address, max 8 bits wide
   :param uint16_t value: value to be written to the register
   :return: success or not
   :retval true: success
   :retval false: failed

Reference Implementation
========================
`My2C <https://github.com/waterloo-rocketry/cansw_sensor/blob/a4ad06b4324a6edf7b92796053f81f6b336b0ab0/my2c.c>`_ in cansw_sensor.