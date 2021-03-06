/* 
 * File:   comms.h
 * Author: Jared Sanson
 *
 * Created on 13 December 2013, 6:55 PM
 */

#ifndef COMMS_H
#define	COMMS_H

#include "drivers/usb/usb.h" // PACKET_SIZE
#include "background/power_monitor.h"
#include "api/clock.h"
#include "api/calendar.h" // MAX_LABEL_LEN, MAX_LOCATION_LEN

#define CMD_PING                0x01
#define CMD_RESET               0x02
#define CMD_SET_LED             0x03

// System debug information
#define CMD_GET_BATTERY_INFO    0x10    // Battery voltage, VDD, levels, status
#define CMD_GET_CPU_INFO        0x11    // Osc freq, systick, utilization, time spent in sleep
#define CMD_GET_NEXT_MESSAGE    0x12    // Next debug message in the buffer

// Display interface
#define CMD_QUERY_DISPLAY       0x20    // Returns parameters of the display
#define CMD_SET_DISPLAY_POWER   0x21    // Turns the display on or off
#define CMD_DISPLAY_LOCK        0x22    // Stops the display buffer from being updated by the system
#define CMD_DISPLAY_UNLOCK      0x23
#define CMD_DISPLAY_WRITEBUF    0x24    // Update the display buffer with some custom data
#define CMD_DISPLAY_READBUF     0x25    // Retrieve the contents of the display buffer

// Sensors
#define CMD_QUERY_SENSORS       0x30    // Return a list of available sensors
#define CMD_SET_SENSOR_ENABLE   0x31    // Enable/disable the specified sensor
#define CMD_GET_SENSOR_DATA     0x32    // Retrieve processed data for the given sensor

// Time & Date
#define CMD_GET_DATETIME        0x40
#define CMD_SET_DATETIME        0x41

// Calendar
#define CMD_CLEAR_CALENDAR      0x50
#define CMD_ADD_CALENDAR_EVT    0x51
#define CMD_GET_CALENDAR_INFO   0x52
#define CMD_GET_CALENDAR_EVT    0x53

// Error codes
#define ERR_OK                  0x00
#define ERR_UNKNOWN             0x01
#define ERR_OUT_OF_RAM          0x10
#define ERR_NOT_IMPLEMENTED     0x11
#define ERR_INVALID_INDEX       0x12
#define ERR_INVALID_PARAM       0x13


// The following structs have __may_alias__ defined to tell the compiler
// it's ok to use them for aliasing a buffer.
// IMPORTANT: You must make sure the buffer has at least as many bytes
// as these structs!!! (Should be 64 bytes)

typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    union {
        byte bytes[62];
        uint16 value;
    };
} generic_packet_t;

typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    uint16 level;
    uint16 voltage;

    charge_status_t charge_status;
    power_status_t power_status;
    battery_status_t battery_status;

    uint8 bq25010_status;
} battery_info_t;

typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    uint16 systick;
    //TODO: add more fields
} cpu_info_t;

typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    // Device-dependant constants
    uint16 width;
    uint16 height;
    uint16 bpp;

    // Current display parameters
    uint16 display_on;
} display_query_t;

#define DISP_CHUNK_SIZE 32
typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    byte state;
    uint16 offset;
    byte buf[DISP_CHUNK_SIZE];
} display_chunk_t;

typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    uint16 count;
    uint8 sensors[32];
} sensor_query_t;

typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    uint16 size;        // Number of bytes returned by the sensor (0 if sensor is disabled)

    byte data[PACKET_SIZE-4];     // Raw data, format depends on the sensor type
} sensor_packet_t;

#define DEBUG_MESSAGE_MAXLEN  PACKET_SIZE-4
typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    uint16 len;

    char message[DEBUG_MESSAGE_MAXLEN];
} message_packet_t;

typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    uint8 hour;             // 0-24
    uint8 minute;           // 0-59
    uint8 second;           // 0-59

    uint8 day_of_week;      // 0:Sunday, 1:Monday, ..., 6:Saturday
    uint8 day;              // 1-31* depending on month
    uint8 month;            // 1-12
    uint8 year;             // 0-99
} datetime_packet_t;

typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    int16 index;
    byte event_type; // calendar_event_type_t

    char label[MAX_LABEL_LEN];
    char location[MAX_LOCATION_LEN];
    uint16 color; // reserved, color_t

    // Weekly calendar events
    uint8 dow; // dow_t
    uint16 hr;
    uint16 min;

} calendar_event_packet_t;

typedef struct __attribute__((packed, __may_alias__)) {
    byte command;
    byte error;

    uint16 num_events;
} calendar_info_packet_t;

typedef enum {
    cmDisconnected,     // USB not connected
    cmIdle,             // Not doing anything, use a low priority
    cmActive            // Actively being used, use a high priority
} comms_status_t;

void InitializeComms();

extern bool usb_connected;
extern comms_status_t comms_status;

#endif	/* COMMS_H */

