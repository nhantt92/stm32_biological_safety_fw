#include "main_screen.h"
#include "u8g2.h"
#include "key.h"
#include <stdlib.h>
#include "system.h"
#include "config.h"
#include "sm5852.h"

MAIN_SCREEN_T main_scr;
uint8_t blink = 1;

void main_screen_init(void)
{
    u8g2_ClearBuffer(&u8g2);
    main_scr.tick = HAL_GetTick();
}

void Lamp_Status(uint8_t lampStatus)
{
    u8g2_SetFont(&u8g2, u8g2_font_micro_tr);
    u8g2_DrawStr(&u8g2, 9, 33, "NEON");
    if (lampStatus)
    {
        u8g2_DrawBitmap(&u8g2, 0, 33, bmp_neon_on.width / 8, bmp_neon_on.height, bmp_neon_on.data);
    }
    else
    {
        u8g2_DrawBitmap(&u8g2, 0, 33, bmp_neon_off.width / 8, bmp_neon_off.height, bmp_neon_off.data);
    }
}

void UV_Status(uint8_t uvStatus)
{
    u8g2_SetFont(&u8g2, u8g2_font_micro_tr);
    u8g2_DrawStr(&u8g2, 45, 33, "UV");
    if (uvStatus)
    {
        u8g2_DrawBitmap(&u8g2, 32, 33, bmp_uv_on.width / 8, bmp_uv_on.height, bmp_uv_on.data);
    }
    else
    {
        u8g2_DrawBitmap(&u8g2, 32, 33, bmp_uv_off.width / 8, bmp_uv_off.height, bmp_uv_off.data);
    }
}

void Fan_Status(uint8_t fanStatus)
{
    u8g2_SetFont(&u8g2, u8g2_font_micro_tr);
    u8g2_DrawStr(&u8g2, 75, 33, "FAN");
    /* ----------------Operation of the Fan--------------- */
    (main_scr.fanRotate & fanStatus) ? u8g2_DrawBitmap(&u8g2, 68, 38, bmp_fan_rotate.width / 8, bmp_fan_rotate.height, bmp_fan_rotate.data)
                       : u8g2_DrawBitmap(&u8g2, 68, 38, bmp_fan_origin.width / 8, bmp_fan_origin.height, bmp_fan_origin.data);
}

void Socket_Status(uint8_t sStatus)
{
    u8g2_SetFont(&u8g2, u8g2_font_micro_tr);
    u8g2_DrawStr(&u8g2, 101, 33, "SOCKET");
    if (sStatus)
    {
        u8g2_DrawBitmap(&u8g2, 98, 34, bmp_socket_on.width / 8, bmp_socket_on.height, bmp_socket_on.data);
        u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf);
        u8g2_DrawStr(&u8g2, 108, 62, "ON");
    }
    else
    {
        u8g2_DrawBitmap(&u8g2, 98, 34, bmp_socket_off.width / 8, bmp_socket_off.height, bmp_socket_off.data);
        u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf);
        u8g2_DrawStr(&u8g2, 105, 62, "OFF");
    }
}

void Filter_Pcent(float pressure, uint16_t threshold)
{
    uint8_t percent;
    uint8_t buff[5];
    uint8_t warnBuff[30];
    uint8_t line;
    if(pressure > threshold) pressure = threshold;
    percent = (uint8_t)((pressure * 100)/((float)threshold));
    percent = fabs(100 - percent);
    line = percent * 50 / 100;
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);
    if((percent == 100) && (percent >= 70))
        u8g2_DrawStr(&u8g2, 17, 21, "The filter is good!");
    else if((percent <= 70) && (percent > 30))
        u8g2_DrawStr(&u8g2, 17, 21, "The filter medium!");
    else if((percent <= 30) && (percent > 10))
        u8g2_DrawStr(&u8g2, 17, 21, "The filter low!");
    else
    {
        u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
        if(main_scr.warnCnt > F_WARNING)
        {
            u8g2_DrawStr(&u8g2, 17, 21, "Please change filter!");
        }
        else
        {
            u8g2_DrawStr(&u8g2, 17, 21, "                     ");
        }
    }
    u8g2_DrawStr(&u8g2, 7, 10, "Filter:");
    u8g2_DrawFrame(&u8g2, 43, 2, 54, 10);
    sprintf(buff, "%d %%", percent);
    u8g2_DrawStr(&u8g2, 98, 10, buff);
    u8g2_DrawBox(&u8g2, 45, 4, line, 6);   
}

void Horizontal(void)
{
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawFrame(&u8g2, 0, 24, 128, 40);
    u8g2_DrawHLine(&u8g2, 0, 36, 128);
    u8g2_DrawHVLine(&u8g2, 32, 24, 40, 1);
    u8g2_DrawHVLine(&u8g2, 64, 24, 40, 1);
    u8g2_DrawHVLine(&u8g2, 96, 24, 40, 1);
}

/* Management devices|: Neon, UV, Fan, Socket, filter status */
void Main_Screen_Manage(void)
{
    if (HAL_GetTick() - main_scr.tick > 200)
    {
        main_scr.fanRotate = ~main_scr.fanRotate;
        if(++main_scr.warnCnt > F_WARNING*2) main_scr.warnCnt = 0;
        main_scr.tick = HAL_GetTick();
    }
    u8g2_ClearBuffer(&u8g2);
    Lamp_Status(dev.status.lamp);
    UV_Status(dev.status.uv);
    Fan_Status(dev.status.fan);
    Socket_Status(dev.status.socket);
    Horizontal();
    Filter_Pcent(sm5852_1.pressure, sys_cfg.pressureVal);
}
