#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"

#define BUZZER_PORT GPIOB
#define BUZZER_PIN GPIO_PIN_8

#define BUZZER_ON() HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, SET)
#define BUZZER_OFF() HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, RESET)
#define BUZZER_TOGGLE() HAL_GPIO_TogglePin(BUZZER_PORT, BUZZER_PIN)

#define SHORT_BEEP 	500
#define LONG_BEEP 	1500
#define ALARM		800

typedef enum
{
	STATE_SB,
	STATE_LB,
	STATE_AL,
	STATE_IDLE
} BUZZER_STATE;

typedef struct
{
	uint32_t tick;
	uint8_t state;
	uint8_t short_Status;
	uint8_t long_Status;
	uint8_t alarm_Status;
} BUZZER_T;

void buzzer_short_beep(void);
void buzzer_long_beep(void);
void buzzer_alarm_start(void);
void buzzer_alarm_stop(void);
void handle_buzzer(void);
void buzzer_manage(uint8_t status);
void buzzer_init(void);

#endif