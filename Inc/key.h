#ifndef __KEY_H_
#define __KEY_H_

#include "main.h"

#define 	NEON_PIN 		GPIO_PIN_10
#define 	UV_PIN 			GPIO_PIN_11
#define 	FAN_PIN 		GPIO_PIN_12
#define 	SOCKET_PIN		GPIO_PIN_13

#define		NEON_In() 		HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10)
#define 	UV_In() 		HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)
#define 	FAN_In() 		HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12)
#define 	SOCKET_In() 	HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)

#define NUM_OF_SAMPLE_KEY 5
#define NUM_KEY 4

typedef enum
{
	KEY_NEON = 0,
	KEY_UV,
	KEY_FAN,
	KEY_SOCKET
} KEY_NAME;

typedef enum
{
	KEY_RELEASE = 0,
	KEY_PRESS = 1
} KEY_EVENT;

typedef struct
{
	uint8_t pressed;
	uint8_t press;
	uint8_t waitRelease;
	uint8_t cnt;
} KEY_EXT;

typedef struct
{
	KEY_NAME name;
	KEY_EXT ext[NUM_KEY];
	uint32_t tick;
} KEY_STRUCT;

typedef enum
{
	NONE = 0,
	NEON,
	UV,
	FAN,
	SOCKET
} BUTTON_ID;

void KeyInit(void);
void KeyGet(uint8_t port_id);
BUTTON_ID KeyProcess(void);
void KeyManage(void);

#endif /* __KEY_H_ */