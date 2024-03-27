SPI Driver
**********

Features
========
- SPI register read/write function
- Chip select signal toggle by GPIO driver

SPI Controller Functions
========================
Initialization
--------------
.. code-block:: c
				
	void spi_init(uint8_t freq);

| Initialize SPI module and set up pins.
| Parameters:
| - **freq**, frequency divider, exact meaning TBD

Transmit
--------
.. code-block:: c

	void spi_tx(uint8_t data);

| Transmit SPI
| Parameters:
| - **data**, 8 bit data to write

Receive
-------
.. code-block:: c

	uint8_t spi_rx(void);

| Receive SPI
| Return value: Received byte

Write 8-bit Register
--------------------
.. code-block:: c

	void spi_write8(uint8_t reg_addr, uint8_t value);

| Write to a 8-bit register of a SPI peripheral
| Note that toggle CS line have to be done before and after calling this functin
| Parameters:
| - **reg_addr**, 8 bit register address
| - **value**, 8 bit data to be written

Read 8-bit Register
-------------------
.. code-block:: c

	uint8_t spi_read8(uint8_t reg_addr);

| Read to a 8-bit register of a SPI peripheral
| Note that toggle CS line have to be done before and after calling this functin
| Parameters:
| - **reg_addr**, 8 bit register address
| Return value: data read from the register
