#include "filter_data.h"

DATA_FILTER filt_data;

void Filter_Data_Init(void)
{
    filt_data.val = 350; //Read value of the filter from input
    filt_data.tick = HAL_GetTick();
}