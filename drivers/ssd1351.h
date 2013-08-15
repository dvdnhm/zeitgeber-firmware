/*
 * File:   oled/ssd1351.h
 * Author: Jared
 *
 * Created on 5 July 2013, 4:07 PM
 *
 * SSD1351 Display Driver
 */

#ifndef SSD1351_H
#define	SSD1351_H

#include "api/graphics/gfx.h"

// Check that we can communicate with the display
extern bool ssd1351_Test();

// Turn on & initialize the OLED
extern void ssd1351_PowerOn();

// Turn off the OLED (minimum power draw)
extern void ssd1351_PowerOff();



// Turn on the display pixels
extern void ssd1351_DisplayOn();

// Turn off the display pixels (sleep mode)
extern void ssd1351_DisplayOff();

// Controls screen brightness (0-15)
extern void ssd1351_SetContrast(uint8 contrast);

// Clear the screen pixels
extern void ssd1351_ClearScreen();

// Draw pixels to the screen
void ssd1351_UpdateScreen(__eds__ uint16 *buf, uint size);

// Set the current cursor position
void ssd1351_SetCursor(uint x, uint y) ;

// Fill the screen with a colour
void ssd1351_FillScreen(color_t c) ;

#endif	/* SSD1351_H */