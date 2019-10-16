#ifndef __INPUT_H
#define __INPUT_H

#include "main.h"

#define In_GPIO_Port GPIOC
#define In1_Pin GPIO_PIN_0
#define In2_Pin GPIO_PIN_1
#define In3_Pin GPIO_PIN_2
#define In4_Pin GPIO_PIN_3
#define In5_Pin GPIO_PIN_4
#define In6_Pin GPIO_PIN_5

#define In1()  HAL_GPIO_ReadPin(In_GPIO_Port, In1_Pin)
#define In2()  HAL_GPIO_ReadPin(In_GPIO_Port, In2_Pin)
#define In3()  HAL_GPIO_ReadPin(In_GPIO_Port, In3_Pin)
#define In4()  HAL_GPIO_ReadPin(In_GPIO_Port, In4_Pin)
#define In5()  HAL_GPIO_ReadPin(In_GPIO_Port, In5_Pin)
#define In6()  HAL_GPIO_ReadPin(In_GPIO_Port, In6_Pin)

#define NUM_OF_SAMPLE_INPUT 5
#define NUM_INPUT 6

typedef enum {
	IN1 = 0,
	IN2,
	IN3,
	IN4,
	IN5,
	IN6
}INPUT_NAME;

typedef enum {
	IN_RELEASE = 0,
	IN_ACTIVE = 1
}INPUT_EVENT;

typedef struct {
	uint8_t actived;
	uint8_t active;
	uint8_t waitRelease;
	uint8_t cnt;
}INPUT_EXT;

typedef struct {
	uint8_t name;
	INPUT_EXT ext[NUM_INPUT];
	uint32_t tick;
}INPUT_STRUCT;

typedef enum {
	NO_INPUT,
	IN1_ON,
	IN1_OFF,
	IN2_ON,
	IN2_OFF,
	IN3_ON,
	IN3_OFF,
	IN4_ON,
	IN4_OFF,
	IN5_ON,
	IN5_OFF,
	IN6_ON,
	IN6_OFF
}INPUT_ID;

void Input_Init(void);
void Input_Get(uint8_t port_id);
uint8_t Input_Process(void);
void Input_Manage(void);

// void Input_Neon(void);
// void Input_Uv(void);
// void Input_Fan(void);
// void Input_Socket(void);

#endif