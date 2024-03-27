GPIO Driver
***********

Macros
======

Output enable
-------------
.. code-block:: c

	gpio_enable_output(port, pin)

| Initialize a GPIO pin to be used digital output.
| Parameters:
| - **pin**, MCU port, a letter e.g. *A*, *B*
| - **pin**, Pin number

Input enable
------------
.. code-block:: c

	gpio_enable_input(port, pin)

| Initialize a GPIO pin to be used digital input.
| Parameters:
| - **pin**, MCU port, a letter e.g. *A*, *B*
| - **pin**, Pin number

Set output value
----------------
.. code-block:: c

	gpio_set_output(port, pin, value)

| Set a GPIO digital output pin to output a value.
| Parameters:
| - **pin**, MCU port, a letter e.g. *A*, *B*
| - **pin**, Pin number
| - **value**, Value to be set

Read input value
----------------
.. code-block:: c

	gpio_get_input(port, pin)

| Read digital input from a pin
| Parameters:
| - **pin**, MCU port, a letter e.g. *A*, *B*
| - **pin**, Pin number

Why Macros
==========
We want to pass which pin we are operating on without perfrmance penalty, if we use an macro it can just take for example :code:`A,3` as a parameter and use it generate C register write to the register.
