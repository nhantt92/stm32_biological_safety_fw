#include "main_screen.h"
#include "u8g2.h"
#include "key.h"
#include "filter_data.h"
#include <stdlib.h>

u8g2_t u8g2;

MAIN_SCREEN_STR main_scr;
uint32_t Warn_Blink;
uint8_t blink = 1;

void main_screen_init(void)
{
    main_scr.lampStatus = 0;
    main_scr.uvStatus = 0;
    main_scr.fanStatus = 0;
    main_scr.socketStatus = 0;
    main_scr.fanRotate = 0;
    u8g2_ClearBuffer(&u8g2);
    main_scr.tick = HAL_GetTick();
    Warn_Blink = HAL_GetTick();
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
    if ((HAL_GetTick() - main_scr.tick > 200) && main_scr.fanStatus)
    {
        main_scr.fanRotate = ~main_scr.fanRotate;
        main_scr.tick = HAL_GetTick();
    }
    if (main_scr.fanRotate)
    {
        u8g2_DrawBitmap(&u8g2, 68, 38, bmp_fan_rotate.width / 8, bmp_fan_rotate.height, bmp_fan_rotate.data);
    }
    else
    {
        u8g2_DrawBitmap(&u8g2, 68, 38, bmp_fan_origin.width / 8, bmp_fan_origin.height, bmp_fan_origin.data);
    }
}
void Socket_Status(uint8_t sStatus)
{
    if (sStatus)
    {
        u8g2_DrawBitmap(&u8g2, 98, 34, bmp_socket_on.width / 8, bmp_socket_on.height, bmp_socket_on.data);
        u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf);
        u8g2_DrawStr(&u8g2, 108, 62, "ON");
        u8g2_SetFont(&u8g2, u8g2_font_micro_tr);
        u8g2_DrawStr(&u8g2, 101, 33, "SOCKET");
    }
    else
    {
        u8g2_DrawBitmap(&u8g2, 98, 34, bmp_socket_off.width / 8, bmp_socket_off.height, bmp_socket_off.data);
        u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf);
        u8g2_DrawStr(&u8g2, 105, 62, "OFF");
        u8g2_SetFont(&u8g2, u8g2_font_micro_tr);
        u8g2_DrawStr(&u8g2, 101, 33, "SOCKET");
    }
}

/*void Filter_Pcent(uint8_t percent)
{
    uint8_t buff[5];
    uint8_t warnBuff[30];
    uint8_t line;
    line = percent * 50 / 100;
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);
    if((percent <= 100) && (percent >= 70))
        u8g2_DrawStr(&u8g2, 17, 21, "The filter is good!");
    else if((percent <= 70) && (percent > 40))
        u8g2_DrawStr(&u8g2, 17, 21, "The filter medium!");
    else if((percent <= 40) && (percent > 10))
        u8g2_DrawStr(&u8g2, 17, 21, "The filter low!");
    else
    {
        if(HAL_GetTick() - Warn_Blink > 1000)
        {
            blink = !blink;
            printf("Blinkd: %d \n", blink);
            Warn_Blink = HAL_GetTick();
        }
        u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
        blink?u8g2_DrawStr(&u8g2, 17, 21, "Please change filter!"):u8g2_DrawStr(&u8g2, 17, 21, "                           ");
        // blink?sprintf(warnBuff, "Please change filter!"):sprintf(warnBuff, "                 ");
        // if(blink)
        // {
        //     u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
        //     u8g2_DrawStr(&u8g2, 17, 21, "                      ");
        // }
        // else
        // {
        //     u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
        //     u8g2_DrawStr(&u8g2, 17, 21, "Please change filter!");
        // }
    }
    u8g2_DrawStr(&u8g2, 7, 10, "Filter:");
    u8g2_DrawFrame(&u8g2, 43, 2, 54, 10);
    sprintf(buff, "%d %%", percent);
    u8g2_DrawStr(&u8g2, 98, 10, buff);
    u8g2_DrawBox(&u8g2, 45, 4, line, 6); 
        
}*/
void Filter_Val(uint16_t val)
{
    uint8_t perc[3];
    u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
    u8g2_DrawStr(&u8g2, 7, 10, "Filter:");
    u8g2_DrawFrame(&u8g2, 43, 2, 54, 10);
    if (val < Min_Pa)
    {
        u8g2_DrawStr(&u8g2, 98, 10, itoa(Max_Percent, perc, 10)); //Value percent of the filter
        u8g2_DrawBox(&u8g2, 45, 4, Max_Pixel, 6);                 //Value pixel of the filter
        u8g2_DrawStr(&u8g2, 115, 10, "%");
        u8g2_DrawStr(&u8g2, 18, 21, "The filter is new!"); // When Filter = 100%, show string char.
        // printf("filter is good\n");
    }
    if (val >= Min_Pa && val <= Min_Pa * (Max_Pa / Min_Pa - 1))
    {
        for (uint8_t i = 1; i <= (Max_Pa / Min_Pa - 1); i++)
        {
            if ((val >= Min_Pa * i) && (val < Min_Pa * (i + 1)))
                filt_data.count = i;
        }
        filt_data.pc = Max_Percent - (4 * filt_data.count);
        filt_data.px = Max_Pixel - (2 * filt_data.count);
        // filt_data.pc = val * 50 / (Max_Pa/2);
        // filt_data.px = val * 25 / (Max_Pa/2);
        u8g2_DrawStr(&u8g2, 99, 10, itoa(filt_data.pc, perc, 10)); //Value percent of the filter
        u8g2_DrawBox(&u8g2, 45, 4, filt_data.px, 6);               //Value pixel of the filter
        u8g2_DrawStr(&u8g2, 111, 10, "%");
        u8g2_DrawStr(&u8g2, 10, 21, "The filter is working!"); // When Filter = 100%, show string char.
        // printf("filter is working\n");
    }

    if (val >= Max_Pa)
    {
        u8g2_DrawStr(&u8g2, 99, 10, itoa(Min_Percent, perc, 10)); //Value percent of the filter
        u8g2_DrawBox(&u8g2, 45, 4, Min_Pixel, 6);                 //Value pixel of the filter
        u8g2_DrawStr(&u8g2, 111, 10, "%");
        // printf("The filter is Maximun!\n");
        if (HAL_GetTick() - filt_data.tick > 1000)
        {
            filt_data.reverse = ~filt_data.reverse;
            filt_data.tick = HAL_GetTick();
        }
        filt_data.reverse ? u8g2_DrawStr(&u8g2, 2, 21, "Please change the filter!")
                          : u8g2_DrawStr(&u8g2, 2, 21, "                         ");
        // When Filter is 20%, show string char.
    }
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
void Main_Screen_Manage(void)
{
    Lamp_Status(main_scr.lampStatus);
    UV_Status(main_scr.uvStatus);
    Fan_Status(main_scr.fanStatus);
    Socket_Status(main_scr.socketStatus);
    Horizontal();
    Filter_Val(filt_data.val); //Function get value from the filter and converter into percent.
    // Filter_Val(500); //Test value of filter
}
