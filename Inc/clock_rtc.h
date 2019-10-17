#ifndef __CLOCK_RTC_H
#define __CLOCK_RTC_H

#include "main.h"
#include <time.h>
#include "stm32f1xx_hal_rtc.h"
#include "stm32f1xx_hal_rtc_ex.h"

#define	RTC_HOLDER	0xA5A6

typedef struct __attribute__((packed)){
	int16_t year;				// year with all 4-digit (2011)
	int8_t month;				// month 1 - 12 (1 = Jan)
	int8_t mday;				// day of month 1 - 31
	int8_t wday;				// day of week 1 - 7 (1 = Sunday)
	int8_t hour;				// hour 0 - 23
	int8_t min;					// min 0 - 59
	int8_t sec;					// sec 0 - 59
} DATE_TIME;

extern RTC_HandleTypeDef hrtc;
extern DATE_TIME localTime;


void RTC_Init(void);
void RTC_Sync(DATE_TIME *dt, uint8_t timeZone);
uint32_t RTC_GetUnixTimestamp(DATE_TIME *dt);
void RTC_CalcTime(DATE_TIME *dt, uint32_t unixTime);
void RTC_Now(DATE_TIME *dt);
int8_t TIME_AddSec(DATE_TIME *t, int32_t sec);
int32_t TIME_GetSec(DATE_TIME *t);
int8_t TIME_FromSec(DATE_TIME *t, int32_t sec);
void RTC_CalendarShow(void);

#endif