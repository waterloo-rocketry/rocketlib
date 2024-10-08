Timer Module Documentation
===========================

Overview
--------
This module provides basic timer functionalities, such as initializing Timer0, handling interrupts, and tracking milliseconds since the timer was started.

.. _timer0_init:

timer0_init
-----------
Initializes Timer0 for time tracking. This function must be called before using any other timer-related functionality.

- Enables Timer0 interrupt.
- Sets the timer to 8-bit mode.
- Driven by a 500 kHz clock.
- Ensures the timer is synchronized to the system clock.

.. code-block:: c

   void timer0_init(void);

.. _timer0_handle_interrupt:

timer0_handle_interrupt
------------------------
This function should be called from the main Interrupt Service Routine (ISR) whenever the Timer0 interrupt is triggered. It updates the internal time counter. Note that this function does **not** clear the interrupt flag (`PIR3bits.TMR0IF`); this must be done at the ISR level.

.. code-block:: c

   void timer0_handle_interrupt(void);

.. _millis:

millis
------
Returns the number of milliseconds since `timer0_init()` was called. This is a safe function that disables interrupts momentarily to return a consistent value.

.. code-block:: c

   uint32_t millis(void);

Definitions and Notes
---------------------
- **ISR Responsibility**: The function `timer0_handle_interrupt()` does not clear the Timer0 interrupt flag. This is the responsibility of the top-level ISR.
- **Time Precision**: The module keeps track of milliseconds, and the value returned by `millis()` reflects the time passed since the timer was initialized.
