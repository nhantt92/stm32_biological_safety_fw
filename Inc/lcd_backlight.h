#ifndef __LCD_BACKLIGHT_H__
#define __LCD_BACKLIGHT_H__

#include "main.h"

extern TIM_HandleTypeDef htim1;

void MX_TIM1_Init(void);
void Set_Backlight(uint8_t percent);
void BL_Off(void);
void BL_On(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

#endif