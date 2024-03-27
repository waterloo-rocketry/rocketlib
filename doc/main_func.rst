Appendix A: Main Function
=========================

The main function serves as the entry point for the CAN boards firmware.

Implementation
--------------
1. Calls the device specific initialization function (e.g. :ref:`pic18-init` function) :code:`mcu_init()` to initialize the chip.
2. Calls user defined :code:`pin_init()` function to setup PPS registers.
3. Initialize timer for :code:`millis()` function.
4. Initialize CAN(controller and TX buffer)
5. Calls user defined :code:`peripheral_init()` function to initialize other peripherals. (e.g. I2C, SPI)
6. Start watchdog timer (PIC18 WDT starts by default)
7. Start the main loop

Main loop
---------
The following steps are always running

1. Feed the watchdog timer
2. Check if the fail safe clock monitor has triggered, if so then set it back to use external crystal
3. Reset if X ms has passed since last time received a CAN message
4. Calls :code:`loop()` function
5. if 20 ms has passed since last time, calls :code:`periodic_20ms()`

Justifications
--------------
The PPS setting function being implemented by user because it's hard to pass a pin number as a parameter to a peripheral init function.

