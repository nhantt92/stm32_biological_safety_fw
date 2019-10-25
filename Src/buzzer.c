#include "buzzer.h"
#include "key.h"
#include "config.h"

BUZZER_T buzzer;

void buzzer_short_beep(void)
{
	buzzer.state = STATE_SB;
	if(sys_cfg.speaker)
	{
		BUZZER_ON();
		buzzer.tick = HAL_GetTick();
	}
}

void buzzer_long_beep(void)
{
	buzzer.state = STATE_LB;
	if(sys_cfg.speaker)
	{
		BUZZER_ON();
		buzzer.tick = HAL_GetTick();
	}
}

void buzzer_alarm_start(void)
{
	buzzer.state = STATE_AL;
}

void buzzer_alarm_stop(void)
{
	buzzer.state = STATE_IDLE;
	BUZZER_OFF();
}

void handle_buzzer(void)
{
	uint32_t delta = HAL_GetTick() - buzzer.tick;
	switch (buzzer.state)
	{
		case STATE_SB:
			if(delta > SHORT_BEEP)
			{
				BUZZER_OFF();
				buzzer.state = STATE_IDLE;
			}
			break;
		case STATE_LB:
			if (delta > LONG_BEEP)
			{
				BUZZER_OFF();
				buzzer.state = STATE_IDLE;
			}
			break;
		case STATE_AL:
			if (delta > ALARM)
			{
				BUZZER_TOGGLE();
				buzzer.tick = HAL_GetTick();
			}
			break;
		default:
			break;
	}
}

// void buzzer_manage(uint8_t status)
// {
// 	switch (status)
// 	{
// 	case 1:
// 		//buzzer_short_beep();
// 		handle_buzzer();
// 		break;
// 	case 2:
// 		//buzzer_long_beep();
// 		handle_buzzer();
// 	case 3:
// 		//buzzer_alarm_start();
// 		handle_buzzer();
// 	default:
// 		break;
// 	}
// }

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
	BUZZER_OFF();
}