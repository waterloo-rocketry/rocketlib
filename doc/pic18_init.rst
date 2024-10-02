.. _pic18-init:

PIC18 Specific Initialization  (Not Implemented)
================================================
The PIC18 initialization handles the initialization of PIC18F26K83, the initialization includes clock settings. However this function does not handle setting the Peripheral Pin Select (PPS) Registers and CAN initialization.

Function Prototype
------------------
.. code-block:: c

   void mcu_init(void);

Steps
-----
1. Set to use external oscillator
2. Initialize IO register default value

