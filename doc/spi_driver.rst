SPI Driver  (Not Implemented)
*****************************

Features
========
- SPI register read/write function
- Chip select signal toggle by GPIO driver

SPI Controller Functions
========================

Initialization
--------------
.. c:function:: void spi_init(uint8_t freq)

   Initialize SPI module and set up pins.

   :param uint8_t freq: frequency divider, exact meaning TBD

Transmit
--------
.. c:function:: void spi_tx(uint8_t data)

   Transmit SPI

   :param uint8_t data: 8 bit data to write

Receive
-------
.. c:function:: uint8_t spi_rx(void)

   Receive SPI

   :return: Received byte
   :rtype: uint8_t

Write 8-bit Register
--------------------
.. c:function:: void spi_write8(uint8_t reg_addr, uint8_t value)

   Write to a 8-bit register of a SPI peripheral.
   Note that toggle CS line have to be done before and after calling this function.

   :param uint8_t reg_addr: 8 bit register address
   :param uint8_t value: 8 bit data to be written

Read 8-bit Register
-------------------
.. c:function:: uint8_t spi_read8(uint8_t reg_addr)

   Read from a 8-bit register of a SPI peripheral.
   Note that toggle CS line have to be done before and after calling this function.

   :param uint8_t reg_addr: 8 bit register address
   :return: data read from the register
   :rtype: uint8_t
