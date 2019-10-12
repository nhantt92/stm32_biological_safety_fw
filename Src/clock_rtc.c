#include "clock_rtc.h"

DATE_TIME localTime;
RTC_HandleTypeDef hrtc;

uint32_t timeRTC;

void RTC_Init(void)
{
    timeRTC = HAL_GetTick();
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != RTC_HOLDER) 
    {
        localTime.year = 2019;
        localTime.month = 10;
        localTime.mday = 12;
        localTime.hour = 11 - 7;
        localTime.min = 30;
        localTime.sec = 00;
        RTC_Sync(&localTime, +7);
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, RTC_HOLDER);   
    }
    else
    {
        if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
        {
        }
        if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
        {
        }
        /* Clear source Reset Flag */
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    HAL_RTCEx_SetSecond_IT(&hrtc);
}

uint32_t RTC_GetUnixTimestamp(DATE_TIME *dt)
{
	struct tm t;
	time_t t_of_day;
	t.tm_year = dt->year - 1900;
	t.tm_mon = dt->month - 1; 				// Month, 0 - jan
	t.tm_mday = dt->mday; 					// Day of the month
	t.tm_hour = dt->hour;
	t.tm_min = dt->min;
	t.tm_sec = dt->sec;
	t.tm_isdst = -1; 						// Is DST on? 1 = yes, 0 = no, -1 = unknown
	t_of_day = mktime(&t);
	return (uint32_t)t_of_day; 
}

void RTC_Sync(DATE_TIME *dt, uint8_t timeZone)
{
	uint32_t tt;
	uint32_t number;
    __HAL_RTC_SECOND_DISABLE_IT(&hrtc, RTC_IT_SEC);
	tt = RTC_GetUnixTimestamp(dt);
	tt += timeZone * 60 * 60;
    RTC_WriteTimeCounter(&hrtc, (uint32_t)tt) ;  // ghi vao thanh ghi gia tri Timestamp
	__HAL_RTC_SECOND_ENABLE_IT(&hrtc, RTC_IT_SEC);
}

void RTC_CalcTime(DATE_TIME *dt, uint32_t unixTime)
{
	struct tm *gt;
	time_t epoch;
	epoch = unixTime;
	gt = localtime(&epoch);			
	dt->sec = gt->tm_sec;
	dt->min = gt->tm_min;
	dt->hour = gt->tm_hour;
	dt->mday = gt->tm_mday;
	dt->wday = gt->tm_wday + 1;				// tm_wday 0 - 6 (0 = sunday)
	dt->month = gt->tm_mon + 1;				// tm_mon 0 - 11 (0 = Jan)
	dt->year = gt->tm_year + 1900;		    // tm_year = current year - 1900
}

void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
    volatile uint32_t number;
    number = RTC_ReadTimeCounter(hrtc);
    RTC_CalcTime(&localTime, number); 
}

void RTC_Now(DATE_TIME *dt)
{
	__HAL_RTC_SECOND_DISABLE_IT(&hrtc, RTC_IT_SEC);
	*dt = localTime;
	__HAL_RTC_SECOND_ENABLE_IT(&hrtc, RTC_IT_SEC);
}


void RTC_CalendarShow(void)
{
    if(HAL_GetTick() - timeRTC > 1000)
    {
        printf("Date: %02d: %02d: %04d \n", localTime.mday, localTime.month, localTime.year);
        printf("Time: %02d: %02d: %02d \n", localTime.hour, localTime.min, localTime.sec);
        timeRTC = HAL_GetTick();
    }
}

// add number of secs to a DATE_TIME struct
int8_t TIME_AddSec(DATE_TIME *t, int32_t sec)
{
	if(t == NULL) return -1;
	if(sec == 0) return 0;	
	TIME_FromSec(t, TIME_GetSec(t) + sec);	
	return 0;
}

// find the number of sec since 00:00 hours, Jan 1, 1970 UTC
int32_t TIME_GetSec(DATE_TIME *t)
{
	struct tm tx;
	if(t == NULL) return -1;	
	tx.tm_sec = t->sec;
	tx.tm_min = t->min;
	tx.tm_hour = t->hour;
	tx.tm_mday = t->mday;
	tx.tm_mday = t->mday;
	tx.tm_wday = t->wday - 1;
	tx.tm_mon = t->month - 1;
	tx.tm_year = t->year - 1900;
	tx.tm_isdst = 0;
	return mktime(&tx);
}

int8_t TIME_FromSec(DATE_TIME *t, int32_t sec)
{
	struct tm *gt;
	if(t == NULL) return -1;
	gt = localtime((const time_t*)&sec);
	t->sec = gt->tm_sec;
	t->min = gt->tm_min;
	t->hour = gt->tm_hour;
	t->mday = gt->tm_mday;
	t->wday = gt->tm_wday + 1;			// tm_wday 0 - 6 (0 = sunday)
	t->month = gt->tm_mon + 1;			// tm_mon 0 - 11 (0 = Jan)
	t->year = gt->tm_year + 1900;		// tm_year = current year - 1900
	return 0;
}