#include "buzzer.h"
#include "key.h"

extern BUZZER_T buzzer;

void buzzer_short_beep(void)
{
	buzzer.state = STATE_SB;
	BUZZER_ON();
}

void buzzer_long_beep(void)
{
	buzzer.state = STATE_LB;
	BUZZER_ON();
}

void buzzer_alarm_start(void)
{
	buzzer.state = STATE_AL;
}

// void buzzer_alarm_stop(void)
// {
// 	buzzer.state = STATE_IDLE;
// 	BUZZER_OFF();
// }

void handle_buzzer(void)
{
	uint32_t delta = HAL_GetTick() - buzzer.tick;
	switch (buzzer.state)
	{
	case STATE_SB:
		if ((delta > SHORT_BEEP) && buzzer.short_Status)
		{
			BUZZER_OFF();
			buzzer.short_Status = 0;
			// printf("Short_Buzzer is off , %d ms\n", delta);
		}
		break;
	case STATE_LB:
		if ((delta > LONG_BEEP) && buzzer.long_Status)
		{
			BUZZER_OFF();
			buzzer.long_Status = 0;
			// printf("Long_Buzzer is off, %d ms \n", delta);
		}
		break;
	case STATE_AL:
		if ((delta > ALARM) && buzzer.alarm_Status)
		{
			BUZZER_TOGGLE();
			buzzer.tick = HAL_GetTick();
			buzzer.cnt++;
			// printf("Alarm_Buzzer is toggling, t = %d\n", buzzer.cnt);
			if (buzzer.cnt >= 15) // Setup time buzzer toggle
			{
				buzzer.alarm_Status = 0;
				// printf("Alarm_Buzzer is off, t= %d \n", buzzer.cnt);
			}
		}
		break;
	default:
		break;
	}
}

void buzzer_manage(uint8_t status)
{
	switch (status)
	{
	case 1:
		buzzer_short_beep();
		handle_buzzer();
		break;
	case 2:
		buzzer_long_beep();
		handle_buzzer();
	case 3:
		buzzer_alarm_start();
		handle_buzzer();
	default:
		break;
	}
}
void buzzer_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = BUZZER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	buzzer.cnt = 0;
	buzzer.short_Status = 1;
	buzzer.long_Status = 2;
	buzzer.alarm_Status = 3;

	buzzer.tick = HAL_GetTick();
	BUZZER_OFF();
}