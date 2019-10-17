#include "input.h"
#include "output.h"
#include "main_screen.h"
#include "info_screen.h"

INPUT_STRUCT input;
extern uint8_t toggle;

void Input_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = In1_Pin | In2_Pin | In3_Pin | In4_Pin | In5_Pin | In6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    for (input.name = 0; input.name < NUM_INPUT; input.name++)
    {
        input.ext[input.name].active = input.ext[input.name].actived = IN_RELEASE;
        input.ext[input.name].waitRelease = 0;
        input.ext[input.name].cnt = 0;
    }
    input.tick = HAL_GetTick();
}

void Input_Get(uint8_t port_id)
{
    uint16_t inputGet;
    switch (port_id)
    {
    case 0:
        inputGet = In1();
        if (inputGet == !In1_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 1:
        inputGet = In2();
        if (inputGet == !In2_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 2:
        inputGet = In3();
        if (inputGet == !In3_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 3:
        inputGet = In4();
        if (inputGet == !In4_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 4:
        inputGet = In5();
        if (inputGet == !In5_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 5:
        inputGet = In6();
        if (inputGet == !In6_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    }
    if (input.ext[port_id].active != input.ext[port_id].actived)
    {
        ++(input.ext[port_id].cnt);
        if (input.ext[port_id].cnt == NUM_OF_SAMPLE_INPUT)
        {
            input.ext[port_id].actived = input.ext[port_id].active;
            input.ext[port_id].cnt = 0;
        }
    }
    else
    {
        input.ext[port_id].cnt = 0;
    }
}

// uint8_t KeyPress(uint8_t id)
// {
// 	return key.ext[id].pressed;
// }

uint8_t Input_Process(void)
{
    uint8_t currentState = 0;
    for (input.name = 0; input.name < NUM_INPUT; input.name++)
    {
        Input_Get(input.name);

        if (input.ext[input.name].actived == IN_ACTIVE)
        {
            if (input.ext[input.name].waitRelease == 0)
            {
                switch (input.name)
                {
                case IN1:
                    currentState = IN1_ON;
                    // printf("INPUT1\n");
                    break;
                case IN2:
                    currentState = IN2_ON;
                    // printf("INPUT2\n");
                    break;
                case IN3:
                    currentState = IN3_ON;
                    // printf("INPUT3\n");
                    break;
                case IN4:
                    currentState = IN4_ON;
                    //  printf("INPUT4\n");
                    break;
                case IN5:
                    currentState = IN5_ON;
                    // printf("INPUT5\n");
                    break;
                case IN6:
                    currentState = IN6_ON;
                    // printf("INPUT6\n");
                default:
                    currentState = NO_INPUT;
                    break;
                }
            }
            input.ext[input.name].waitRelease = 1;
        }
        if (input.ext[input.name].actived == IN_RELEASE)
        {
            if (input.ext[input.name].waitRelease == 1)
            {
                switch (input.name)
                {
                case IN1:
                    currentState = IN1_OFF;
                    break;
                case IN2:
                    currentState = IN2_OFF;
                    break;
                case IN3:
                    currentState = IN3_OFF;
                    break;
                case IN4:
                    currentState = IN4_OFF;
                    break;
                case IN5:
                    currentState = IN5_OFF;
                    break;
                case IN6:
                    currentState = IN6_OFF;
                    break;
                default:
                    currentState = NO_INPUT;
                    break;
                }
            }
            input.ext[input.name].waitRelease = 0;
        }
    }
    return currentState;
}

void Input_Manage(void)
{
    if (HAL_GetTick() - input.tick > 1)
    {
        switch (Input_Process())
        {
        case IN1_ON:
            toggle = 0;
            Info_Scr.Door_Lv0 = 1;
            // printf("IN1 ON\n");
            break;
        case IN2_ON:
        toggle =0;
            Info_Scr.Door_Lv1 = 1;
            // printf("IN2 ON\n");
            break;
        case IN3_ON:
        toggle =0;
            Info_Scr.Door_Lv2 = 1;
            // printf("IN3 ON\n");
            break;
        case IN4_ON:
            // printf("IN4 ON\n");
            break;
        case IN5_ON:
            // printf("IN5 ON\n");
            break;
        case IN6_ON:
            // printf("IN6 ON\n");
            break;
        case IN1_OFF:
        toggle =0;
            Info_Scr.Door_Lv0 = 0;
            // printf("IN1 OFF \n");
            break;
        case IN2_OFF:
        toggle =0;
            Info_Scr.Door_Lv1 = 0;
            // printf("IN2 OFF \n");
            break;
        case IN3_OFF:
        toggle =0;
            Info_Scr.Door_Lv2 = 0;
            // printf("IN3 OFF \n");
            break;
        case IN4_OFF:
            // printf("IN4 OFF \n");
            break;
        case IN5_OFF:
            // printf("IN5 OFF \n");
            break;
        case IN6_OFF:
            // printf("IN6 OFF \n");
            break;
        default:
            break;
        }
        input.tick = HAL_GetTick();
    }
}