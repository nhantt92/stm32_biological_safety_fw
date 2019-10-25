#include "key.h"
#include "u8g2.h"
#include "main_screen.h"
#include "buzzer.h"
#include "system.h"
#include "config.h"

KEY_STRUCT key;
extern uint8_t toggle;

void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	for (key.name = 0; key.name < NUM_KEY; key.name++)
	{
		key.ext[key.name].press = key.ext[key.name].pressed = KEY_RELEASE;
		key.ext[key.name].waitRelease = 0;
		key.ext[key.name].cnt = 0;
	}
	key.tick = HAL_GetTick();
}

void KeyGet(uint8_t port_id)
{
	switch (port_id)
	{
	case 0:
		if (NEON_In() == !NEON_PIN)
			key.ext[port_id].press = KEY_PRESS;
		else
			key.ext[port_id].press = KEY_RELEASE;
		break;
	case 1:
		if (UV_In() == !UV_PIN)
			key.ext[port_id].press = KEY_PRESS;
		else
			key.ext[port_id].press = KEY_RELEASE;
		break;
	case 2:
		if (FAN_In() == !FAN_PIN)
			key.ext[port_id].press = KEY_PRESS;
		else
			key.ext[port_id].press = KEY_RELEASE;
		break;
	case 3:
		if (SOCKET_In() == !SOCKET_PIN)
			key.ext[port_id].press = KEY_PRESS;
		else
			key.ext[port_id].press = KEY_RELEASE;
		break;
	default:
		break;
	}
	if (key.ext[port_id].press != key.ext[port_id].pressed)
	{
		++(key.ext[port_id].cnt);
		if (key.ext[port_id].cnt == NUM_OF_SAMPLE_KEY)
		{
			key.ext[port_id].pressed = key.ext[port_id].press;
			key.ext[port_id].cnt = 0;
		}
	}
	else
	{
		key.ext[port_id].cnt = 0;
	}
}

BUTTON_ID KeyProcess(void)
{
	uint8_t currentState = NONE;
	for (key.name = 0; key.name < NUM_KEY; key.name++)
	{
		KeyGet(key.name);
		if (key.ext[key.name].pressed == KEY_PRESS)
		{
			if (key.ext[key.name].waitRelease == 0)
			{
				switch (key.name)
				{
				case KEY_NEON:
					currentState = NEON;
					break;
				case KEY_UV:
					currentState = UV;
					break;
				case KEY_FAN:
					currentState = FAN;
					break;
				case KEY_SOCKET:
					currentState = SOCKET;
					break;
				default:
					currentState = NONE;
					break;
				}
			}
			key.ext[key.name].waitRelease = 1;
		}
		if (key.ext[key.name].pressed == KEY_RELEASE)
		{
			key.ext[key.name].waitRelease = 0;
		}
	}
	return currentState;
}

uint8_t key_press;

void KeyManage(void)
{
	if (HAL_GetTick() - key.tick >= 5)
	{
		switch (KeyProcess())
		{
		case NEON:
			toggle = 0;
			dev.status.lamp = !dev.status.lamp;
			Device_Save_Status();
			buzzer_short_beep();
			break;
		case UV:
			toggle = 0;
			dev.status.uv = !dev.status.uv;
			dev.status.uv?UV_Init_Time(sys_cfg.uvMode):UV_Clear_Time();
			Device_Save_Status();
			buzzer_short_beep();
			break;
		case FAN:
			toggle = 0;
			dev.status.fan = !dev.status.fan;
			Device_Save_Status();
			buzzer_short_beep();
			break;
		case SOCKET:
			toggle = 0;
			dev.status.socket = !dev.status.socket;
			Device_Save_Status();
			buzzer_short_beep();
			break;
		default:
			break;
		}
		key.tick = HAL_GetTick();
	}
}