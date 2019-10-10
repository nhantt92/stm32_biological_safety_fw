#ifndef __INPUT_H
#define __INPUT_H

#include "main.h"

#define In1_Pin GPIO_PIN_0
#define In1_GPIO_Port GPIOC
#define In2_Pin GPIO_PIN_1
#define In2_GPIO_Port GPIOC
#define In3_Pin GPIO_PIN_2
#define In3_GPIO_Port GPIOC
#define In4_Pin GPIO_PIN_3
#define In4_GPIO_Port GPIOC
#define In5_Pin GPIO_PIN_4
#define In5_GPIO_Port GPIOC
#define In6_Pin GPIO_PIN_5
#define In6_GPIO_Port GPIOC

#define In1()  HAL_GPIO_ReadPin(In1_GPIO_Port, In1_Pin)
#define In2()  HAL_GPIO_ReadPin(In2_GPIO_Port, In2_Pin)
#define In3()  HAL_GPIO_ReadPin(In3_GPIO_Port, In3_Pin)
#define In4()  HAL_GPIO_ReadPin(In4_GPIO_Port, In4_Pin)
#define In5()  HAL_GPIO_ReadPin(In5_GPIO_Port, In5_Pin)
#define In6()  HAL_GPIO_ReadPin(In6_GPIO_Port, In6_Pin)

#define On 0
#define Off 1

void Input_Init(void);
void Input_Neon(void);
void Input_Uv(void);
void Input_Fan(void);
void Input_Socket(void);
void Input_Manage(void);

#endif