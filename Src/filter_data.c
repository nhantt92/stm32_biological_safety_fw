#include "filter_data.h"

DATA_FILTER filt_data;

void Filter_Data_Init(void)
{
    filt_data.val = 300;
    // filt_data.percent = Max_Percent;
    // filt_data.px = Max_Pixel;
}