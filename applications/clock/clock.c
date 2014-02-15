/*
 * File:   applications/clock/clock.c
 * Author: Jared
 *
 * Created on 14 February 2014, 1:39 PM
 *
 * clock test application
 *
 */

////////// Includes ////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "system.h"
#include "api/app.h"
#include "api/api.h"
#include "api/graphics/gfx.h"
#include "util/util.h"
#include "core/kernel.h"
#include "background/power_monitor.h"
#include "peripherals/rtc.h"
#include "applications/clock/clock_font.h"


////////// App Definition //////////////////////////////////////////////////////

void appclock_Initialize();
void appclock_Process();
void appclock_Draw();

application_t appclock = APPLICATION("Clock", appclock_Initialize, appclock_Process, appclock_Draw);

////////// Variables ///////////////////////////////////////////////////////////

////////// Code ////////////////////////////////////////////////////////////////

// Called when CPU initializes 
void appclock_Initialize() {
    appclock.state = asIdle;
}

// Called periodically when state==asRunning
void appclock_Process() {
    while (1) {
        Delay(1000);
    }
}

// Called periodically when isForeground==true (30Hz)
void appclock_Draw() {
    char s[10];

    //SetFontSize(2);

    //RtcTimeToStr(s);
    //DrawString(s, 8,8,WHITE);

    rtc_time_t time = RtcTime();

    int x = 0;
    x = DrawClockInt(x,20, time.hour12);
    x = DrawClockDigit(x,20, CLOCK_DIGIT_COLON);
    x = DrawClockInt(x,20, time.min);
    x = DrawClockDigit(x,20, (time.pm) ? CLOCK_DIGIT_PM : CLOCK_DIGIT_AM);

}
