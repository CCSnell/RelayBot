#pragma once

#include "stdio.h"
#include <stdint.h>
#include <WiFi.h>
#include <WebServer.h> 
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <Preferences.h>
#include "WS_GPIO.h"
#include "WS_Information.h"
#include "WS_Relay.h"
#include "WS_RTC.h"
#include "WS_DIN.h"

extern char ipStr[16];
extern bool Main_Lock_State;  // Main relay controls lock state

void handleRoot();
void handleGetData();
void handleSwitch(uint8_t ledNumber);

void handleSwitch1();
void handleSwitch2();
void handleSwitch3();
void handleSwitch4();
void handleSwitch5();
void handleSwitch6();
void handleSwitch7();
void handleSwitch8();
void handlePulseRelay();
void handleSetChannelPulseSettings();
void handleGetChannelPulseSettings();
void handleSetMainLock();
void handleGetInputStates();
void WIFI_Init();
void WIFI_Loop();
void WebTask(void *parameter);
void loadChannelPulseSettingsFromNVS();
void saveChannelPulseDurationToNVS(uint8_t channel, uint32_t duration);
void saveChannelPulseCountToNVS(uint8_t channel, uint32_t count);

bool parseData(const char* Text, datetime_t* dt, Status_adjustment* Relay, Repetition_event* cycleEvent) ;