#ifndef ROCKETLIB_TIMER_H
#define ROCKETLIB_TIMER_H

#include <stdint.h>

/* Initializes Timer0 for time tracking.
   Must be called before using the timer functions.
*/
void timer0_init(void);

/* Handles Timer0 interrupt.
   Should be called in the main ISR when Timer0 triggers an interrupt.
   Note: Does not clear the interrupt flag `PIR3bits.TMR0IF`.
*/
void timer0_handle_interrupt(void);

/* Returns milliseconds since `timer0_init()` was called.
   Useful for measuring elapsed time.
   
   Return:
   uint32_t: Elapsed time in milliseconds.
*/
uint32_t millis(void);

#endif /* ROCKETLIB_TIMER_H */
