#ifndef __FILTER_DATA_H
#define __FILTER_DATA_H

#include "main.h"

#define Max_Pa 500     //Value filter can setup
#define Min_Pa (Max_Pa/20)       //Value filter min
#define Pa_Inc 15   //Increase value of the filter
#define Min_Percent 20
#define Max_Percent 100
#define Min_Pixel 10
#define Max_Pixel 50
#define Origin 1

typedef struct
{
    volatile uint16_t val;
    uint8_t percent;
    uint8_t px;
    uint8_t pc;
    uint8_t reverse;
    uint32_t tick;
    uint8_t count;
}DATA_FILTER;

extern DATA_FILTER filt_data;

void Filter_Data_Init(void);
#endif