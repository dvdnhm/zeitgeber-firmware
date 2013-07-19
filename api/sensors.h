/*
 * File:   sensors.h
 * Author: Jared
 *
 * Created on 5 July 2013, 4:07 PM
 *
 * Encapsulates all the device's sensors
 */

#ifndef SENSORS_H
#define	SENSORS_H

extern bool InitializeSensors();
extern void ProcessSensors();


// Ambient light level, 0-255
extern uint8 ambient_light;

// Accelerometer
#include "drivers/MMA7455.h"

// Magnetometer
#include "drivers/HMC5883.h"

#endif	/* SENSORS_H */