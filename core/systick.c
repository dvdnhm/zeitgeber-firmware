/*
 * File:   systick.c
 * Author: Jared
 *
 * Created on 5 July 2013, 2:56 PM
 */

#include <timer.h>
#include "system.h"
#include "hardware.h"
#include "systick.h"

#if SYSTICK_PR > 0xFFFF
#error SYSTICK too big
#elif SYSTICK_PR == 0
#error SYSTICK too small
#endif


volatile uint systick = 1;

// Note: since we want to stop the systick interrupt from waking the CPU
// out of sleep mode, we'll need to add the number of ticks spent sleeping somehow?
// or maybe turn down the timer rate while it sleeps in case an asynchronous interrupt
// wakes the processor (eg. accelerometer interrupt)

void systick_init() {
    // Configure a system tick timer with interrupt

    _T1MD = 0; // Enable T1 peripheral

    T1CON = T1_OFF & T1_IDLE_CON & T1_GATE_OFF &
            SYSTICK_PS(SYSTICK_PRESCALER) & T1_SYNC_EXT_OFF & T1_SOURCE_INT;

    TMR1 = 0x0000;
    PR1 = SYSTICK_PR;

    _T1IF = 0;
    _T1IP = 1; // Low priority so it doesn't pre-empt other interrupts
    _T1IE = 1;

    T1CONbits.TON = 1;
}

/*void isr _T1Interrupt() {
    _T1IF = 0;
    systick++; // atomic operation since systick is a uint16
    ClrWdt();
}*/

