/*
 * File:   os.c
 * Author: Jared
 *
 * Created on 19 July 2013, 5:41 PM
 */

////////// Includes ////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "core/kernel.h"
#include "api/graphics/gfx.h"
#include "os.h"
#include "api/app.h"
#include "hardware.h"

#include "drivers/ssd1351.h"
#include "background/comms.h"
#include "background/power_monitor.h"
#include "drivers/MMA7455.h"
#include "util/util.h"
#include "peripherals/gpio.h"
#include "peripherals/cn.h"


////////// GUI Resources ///////////////////////////////////////////////////////

#include "gui/icons/usb.h"
#include "gui/icons/power.h"


////////// Variables ///////////////////////////////////////////////////////////

#define DEBOUNCE_INTERVAL 25 //systicks (ms)

enum { btnReleased=false, btnPressed=true };

bool displayOn = true;

task_t* core_task;
task_t* draw_task;

uint draw_ticks;

uint current_app = 0;

volatile bool lock_display = false;
volatile bool display_frame_ready = false;
volatile int wipe_frame = 0;

// Note: button indicies start at 1
static uint btn_debounce_tick[5];
bool btn_state[5];
bool btn_debouncing[5];

uint sleep_time;
bool auto_screen_off = true;
uint auto_screen_off_interval = 10000; //systicks

////////// Prototypes //////////////////////////////////////////////////////////

void ProcessCore();
void DrawFrame();
void DrawLoop();
void DisplayBootScreen();
void CheckButtons();

void OnBTN1Change(bool btn_pressed);
void OnBTN2Change(bool btn_pressed);
void OnBTN3Change(bool btn_pressed);
void OnBTN4Change(bool btn_pressed);

////////// Methods /////////////////////////////////////////////////////////////

void InitializeOS() {
    ClrWdt();

    // High priority tasks that must be run all the time
    core_task = RegisterTask("Core", ProcessCore);
    core_task->state = tsRun;

    // Drawing, only needs to be run when screen is on
    draw_task = RegisterTask("Draw", DrawLoop);

    // Initialize button interrupts
    _CNIEn(BTN1_CN) = 1;
    _CNIEn(BTN2_CN) = 1;
    _CNIEn(BTN3_CN) = 1;
    _CNIEn(BTN4_CN) = 1;
    cn_register_cb(_CNIDX(BTN1_CN), _PINREF(BTN1), OnBTN1Change);
    cn_register_cb(_CNIDX(BTN2_CN), _PINREF(BTN2), OnBTN2Change);
    cn_register_cb(_CNIDX(BTN3_CN), _PINREF(BTN3), OnBTN3Change);
    cn_register_cb(_CNIDX(BTN4_CN), _PINREF(BTN4), OnBTN4Change);

    uint i;
    for (i=1; i<=4; i++) {
        btn_debounce_tick[i] = 0;
        btn_state[i] = 0;
    }
}

static void reset_auto_screen_off() {
    sleep_time = systick + auto_screen_off_interval;
}

void ScreenOff() {
    AppGlobalEvent(evtScreenOff, NULL);

    accel_SetMode(accStandby);

    // Disable drawing
    draw_task->state = tsStop;

    /*if (foreground_app != NULL) {
        foreground_app->task->state = tsStop;
    }*/

    ssd1351_DisplayOff();
    ssd1351_PowerOff();
    _LAT(LED1) = 0;
    _LAT(LED2) = 0;

    displayOn = false;
}

void ScreenOn() {
    // Draw a frame before fading in
    DrawFrame();
    //_LAT(OL_POWER) = 1;
    UpdateDisplay();

    ssd1351_PowerOn();
    ssd1351_DisplayOn();

    draw_task->state = tsRun;

    AppGlobalEvent(evtScreenOn, NULL);

    displayOn = true;
    reset_auto_screen_off();
}


void ProcessCore() {
    while (1) {
        ProcessPowerMonitor();

        // Reset debouncing when timeout period expires,
        // to prevent lock-up when a roll-over occurrs.
        uint i;
        for (i=1; i<=4; i++) {
            if (btn_debouncing[i] && systick >= btn_debounce_tick[i]) {
                btn_debouncing[i] = false;
            }
        }

        // Turn off screen automatically after some amount of time
        if (auto_screen_off && displayOn && systick > sleep_time) {
            ScreenOff();
        }

        if (displayOn)
            Delay(CORE_PROCESS_INTERVAL);
        else
            Delay(CORE_STANDBY_INTERVAL);
    }
}

static void NextApp() {
    if (current_app <= app_count-2) {
        if (wipe_frame == 0) { //TODO: queue up events instead of ignoring
            current_app++;
            SetForegroundApp(installed_apps[current_app]);

            wipe_frame = +1;
        }
    }
}
static void PrevApp() {
    if (current_app > 0) {
        if (wipe_frame == 0) {
            current_app--;
            SetForegroundApp(installed_apps[current_app]);

            wipe_frame = -1;
        }
    }
}

static inline void OnBTNChange(bool btn_pressed, uint btn) {
    // Assumes btn 1..4

    // De-bouncing
    bool *state = &btn_state[btn];
    uint *tick = &btn_debounce_tick[btn];
    bool *debouncing = &btn_debouncing[btn];

    if (*debouncing && systick < *tick) {
        return; // Event occurred within debounce interval
    }

    // Change state
    *state = btn_pressed;
    *tick = systick + DEBOUNCE_INTERVAL;
    *debouncing = true;

    //printf("btn %d : %d\n", btn, btn_pressed);

    reset_auto_screen_off();

    // Event handling
    if (btn_pressed) {
        if (!displayOn) {
            ScreenOn();
        }
        else {
            switch (btn) {
                case 1:
                    break;
                case 2:
                    PrevApp();
                    break;
                case 3:
                    NextApp();
                    break;
                case 4:
                    ScreenOff();
                    break;
            }
        }

        AppForegroundEvent(evtBtnPress, btn);
    } else {
        AppForegroundEvent(evtBtnRelease, btn);
    }
}
void OnBTN1Change(bool btn_pressed) {
    OnBTNChange(btn_pressed, 1);
}
void OnBTN2Change(bool btn_pressed) {
    OnBTNChange(btn_pressed, 2);
}
void OnBTN3Change(bool btn_pressed) {
    OnBTNChange(btn_pressed, 3);
}
void OnBTN4Change(bool btn_pressed) {
    OnBTNChange(btn_pressed, 4);
}


void BootPrintln(const char* s) {
    static uint32 y = 8;
    DrawString(s, 8, y, WHITE);
    UpdateDisplay();
    y += 10;
}

void DisplayBootScreen() {
    byte y = 8;
    char s[10];
    byte x;
    uint32 i;

    ClrWdt();
    ClearImage();

    ClrWdt();
    BootPrintln("OLED Watch v1.0");
    //DrawString("Booting...", 8, y, WHITE); y += 10;

    
    
    for (i=0; i<1000000; i++) { ClrWdt(); }
}

void DrawFrame() {
    //_LAT(LED1) = 1;

    global_drawop = SRCCOPY;
    SetFontSize(1);
    SetFont(fonts.Stellaris);

    // Draw the wallpaper
    //DrawImage(0,0,wallpaper);
    ClearImage();

    // Draw foreground app
    if (foreground_app != NULL)
        foreground_app->draw();

    // Draw the battery bar
    uint8 w = mLerp(0,100, 0,DISPLAY_WIDTH, battery_level);
    color_t c = WHITE;
    switch (power_status) {
        case pwBattery: {
            switch (battery_status) {
                case batFull: c = SKYBLUE; break;
                case batNormal: c = SKYBLUE; break;
                case batLow: c = RED; break;
                // No need to put batFlat or batNotConnected
                default: break;
            }
            break;
        }
        case pwCharged: c = GREEN; break;
        case pwCharging: c = ORANGE; break;
    }
    // Extra padding at the top of the display to compensate for the bezel
    //DrawBox(0,0, DISPLAY_WIDTH,4, BLACK,BLACK);
    DrawBox(0,0, w,3, c,c);

    // Draw the battery icon
    if (power_status == pwBattery) {
        char s[8];
        sprintf(s, "%3d%%", battery_level);
        //utoa(s, battery_level, 10);
        //int x = DISPLAY_WIDTH - StringWidth(s) - 2;
        int x = DISPLAY_WIDTH - 30;
        DrawImString(s, x,5, WHITE);

    } else {
        if (usb_connected) {
            DrawImage(DISPLAY_WIDTH-USB_WIDTH-2,5, &img_usb);
        } else {
            DrawImage(DISPLAY_WIDTH-POWER_WIDTH-2,6, &img_power);
        }
    }

    // Framerate debug info
//    char s[8];
//    sprintf(s, "%d", draw_ticks);
//    DrawString(s, 4,5, DARKGREEN);
}

// Called periodically
void DrawLoop() {
    static uint scroll = 1;
    
    while (1) {
        uint t1, t2;
        uint next_tick = systick + DRAW_INTERVAL;

        t1 = systick;

        if (!lock_display) {
            display_frame_ready = false;

            DrawFrame();
        
            display_frame_ready = true;

            if (wipe_frame == 0) {
                //_LAT(LED1) = 1;
                UpdateDisplay();
                //_LAT(LED1) = 0;
            } else {
                // Blocking call
                UpdateDisplayWipeIn(wipe_frame);
                wipe_frame = 0;
            }
        }

        t2 = systick;
        draw_ticks = (t2 >= t1) ? (t2 - t1) : 0;

        Delay(DRAW_INTERVAL);
        //WaitUntil(next_tick);
        //Delay(0);
    }
}

