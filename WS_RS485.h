#pragma once

#include <HardwareSerial.h>       // Reference the ESP32 built-in serial port library
#include "WS_GPIO.h"
#include "WS_Relay.h"

#define Extension_CH1       1     // Expansion Channel 1
#define Extension_CH2       2     // Expansion Channel 2
#define Extension_CH3       3     // Expansion Channel 3
#define Extension_CH4       4     // Expansion Channel 4
#define Extension_CH5       5     // Expansion Channel 5
#define Extension_CH6       6     // Expansion Channel 6
#define Extension_CH7       7     // Expansion Channel 7
#define Extension_CH8       8     // Expansion Channel 8
#define Extension_ALL_ON    9     // Expansion ALL ON
#define Extension_ALL_OFF   10    // Expansion ALL OFF



void SetData(uint8_t* data, size_t length);   // Send data from the RS485
void ReadData(uint8_t* buf, uint8_t length);  // Data is received over RS485

void RS485_Analysis(uint8_t *buf);            // External relay control
void RS485_Init();         // Example Initialize the system serial port and RS485
void RS485_Loop();         // Read RS485 data, parse and control relays
void RS485Task(void *parameter);
