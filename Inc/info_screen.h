#ifndef __INFO_SCREEN_H
#define __INFO_SCREEN_H

#include "main.h"
#include "clock_rtc.h"

typedef struct {
    uint8_t dot;
    uint32_t tick;
} INFO_SCREEN_STR;

extern INFO_SCREEN_STR Info_Scr;

void Info_Screen_Init(void);
void RTC_Time(DATE_TIME now);
void ExhaustFilter(float value);
void DownflowFilter(float value);
void InFlow(float value);
void DownFlow(float value);
void Temperature(float value);
void Door(uint8_t DStatus);
void Info_Screen_Manage(void);

#endif