#include "buzzer.h"

BUZZER_T buzzer;

void buzzer_short_beep(void)
{
	buzzer.state =  STATE_SB;
	BUZZER_ON();
	buzzer.tick = HAL_GetTick();
}

void buzzer_long_beep(void)
{
	buzzer.state = STATE_LB;
	BUZZER_ON();
	buzzer.tick = HAL_GetTick();
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
	switch(buzzer.state)
	{
		case STATE_SB:
			if(delta > SHORT_BEEP)
			{
				BUZZER_OFF();
				buzzer.state = STATE_IDLE;
			}
			break;
		case STATE_LB:
			if(delta > LONG_BEEP)
			{
				BUZZER_OFF();
				buzzer.state = STATE_IDLE;
			}
			break;
		case STATE_AL:
			if(delta > ALARM)
			{
				BUZZER_TOGGLE();
				buzzer.tick = HAL_GetTick();
			}
			break;
        // case STATE_IDLE:
        //     BUZZER_OFF();
        //     break;
		default:
            // BUZZER_OFF();
			break;
	}
}

void buzzer_manage(void)
{
    buzzer_alarm_start();
    //handle_buzzer();
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

    BUZZER_OFF();
}