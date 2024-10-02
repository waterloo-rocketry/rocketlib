GPIO Driver (Not Implemented)
*****************************

Macros
======

Output enable
-------------
.. c:macro:: gpio_enable_output(port, pin)

   Initialize a GPIO pin to be used as digital output.

   :param port: MCU port, a letter e.g. A, B
   :param pin: Pin number

Input enable
------------
.. c:macro:: gpio_enable_input(port, pin)

   Initialize a GPIO pin to be used as digital input.

   :param port: MCU port, a letter e.g. A, B
   :param pin: Pin number

Set output value
----------------
.. c:macro:: gpio_set_output(port, pin, value)

   Set a GPIO digital output pin to output a value.

   :param port: MCU port, a letter e.g. A, B
   :param pin: Pin number
   :param value: Value to be set

Read input value
----------------
.. c:macro:: gpio_get_input(port, pin)

   Read digital input from a pin.

   :param port: MCU port, a letter e.g. A, B
   :param pin: Pin number

Why Macros
==========
We want to pass which pin we are operating on without performance penalty. If we use a macro, it can just take for example ``A,3`` as a parameter and use it to generate C register write to the register.
