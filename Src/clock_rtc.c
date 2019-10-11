#include "clock_rtc.h"

void RTC_Init(void)
{
    uint32_t return_time;
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	HAL_RTC_Init(&hrtc);
        if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2) //0x32F2
        {
            return_time = set_time(2019, 10, 11, 13, 25, 30);    //Setup Time
        
        RTC_WriteTimeCounter(&hrtc,return_time) ;  // ghi vao thanh ghi gia tri Timestamp
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);   
        }
        else
        {
            if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
            {
            printf("\n ..");
            }
            if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
            {
            printf("\n ...");
            }
            
            /* Clear source Reset Flag */
            __HAL_RCC_CLEAR_RESET_FLAGS();
        }
    HAL_RTCEx_SetSecond_IT(&hrtc);
}
void RTC_CalendarShow(void)
{
    RTC_DateTypeDef sdatestructureget;
    RTC_TimeTypeDef stimestructureget;
    char sprintfTime[20];
    char sprintfDate[20];
    uint32_t time = RTC_ReadTimeCounter(&hrtc);
    struct tm time_struct =convert_time_stamp(time);

    printf((char *)sprintfTime, "%0.2d:%0.2d:%0.2d", time_struct.tm_hour,time_struct.tm_min,time_struct.tm_sec);
    printf((char *)sprintfDate, ">>>%0.2d-%0.2d-%0.2d<<<",time_struct.tm_mday,time_struct.tm_mon, 2000 + time_struct.tm_year-100);

    printf("\nTimestamp = %d\n", time);
    printf("tm.Second  = %d\n",time_struct.tm_sec);
    printf("tm.Minute  = %d\n",time_struct.tm_min);
    printf("tm.Hour    = %d\n",time_struct.tm_hour);
    printf("tm.Wday    = %d\n",time_struct.tm_wday);  // chu y cac thu trong tuan (0-6)
    printf("tm.Day     = %d\n",time_struct.tm_mday);
    printf("tm.Month   = %d\n",time_struct.tm_mon);
    printf("tm.Year    = %d\n",time_struct.tm_year-100);
    printf("\n-----------------------------------\n");
}