#include "input.h"
#include "output.h"
#include "main_screen.h"

void Input_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = In1_Pin | In2_Pin | In3_Pin | In4_Pin | In5_Pin | In6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Input_Manage(void)
{
    Input_Neon();
    Input_Uv();
    Input_Fan();
    Input_Socket();
}

void Input_Neon(void)
{
    switch (In1())
    {
    case On:
        SW_Neon(1);
        // printf("Neon On\n");
        break;
    default:
        SW_Neon(0);
        // printf("Neon Off\n");
        break;
    }
}
void Input_Uv(void)
{
    switch (In2())
    {
    case On:
        SW_Uv(1);
        break;
    default:
        SW_Uv(0);
        break;
    }
}
void Input_Fan(void)
{
    switch (In3())
    {
    case On:
        SW_Fan(1);
        break;
    default:
        SW_Fan(0);
        break;
    }
}
void Input_Socket(void)
{
    switch(In4())
    {
        case On:
        SW_Socket(1);
        break;
        default:
        SW_Socket(0);
        break;
    }
}