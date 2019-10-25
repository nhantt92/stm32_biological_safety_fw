#include "info_screen.h"
#include "u8g2.h"
#include "user_bitmap.h"
#include "sm5852.h"
#include "system.h"

INFO_SCREEN_STR Info_Scr;

void Info_Screen_Init(void)
{   
    Info_Scr.dot = 0;
    Info_Scr.tick = HAL_GetTick();
}
void RTC_Time(DATE_TIME now)
{   
    uint8_t buff[30];
    if(HAL_GetTick() - Info_Scr.tick > 300)
    {   
        Info_Scr.dot = !Info_Scr.dot;
        Info_Scr.tick = HAL_GetTick();
    }
    Info_Scr.dot?sprintf(buff, "%02d:%02d", now.hour, now.min):sprintf(buff, "%02d %02d", now.hour, now.min);
    u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);
    u8g2_DrawStr(&u8g2, 5, 16, buff);
    u8g2_DrawHVLine(&u8g2, 20, 22, 80, 22);
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
    sprintf(buff, "%02d/%02d/%04d", now.mday, now.month, now.year);
    u8g2_DrawStr(&u8g2, 78, 12, buff);
}

void ExhaustFilter(float value)
{
    uint8_t buff[10];
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
    u8g2_DrawStr(&u8g2, 3, 34, "E.Filter:");
    if(value > 999)
        value = 999;
    sprintf(buff, "%03.0f", value);
    u8g2_DrawStr(&u8g2, 53, 34, buff);
    u8g2_DrawStr(&u8g2, 77, 34, "Pa");
}

void DownflowFilter(float value)
{
    uint8_t buff[10];
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
    u8g2_DrawStr(&u8g2, 3, 43, "D.Filter:");
    if(value > 999)
        value = 999;
    sprintf(buff, "%03.0f", value);
    u8g2_DrawStr(&u8g2, 53, 43, buff);
    u8g2_DrawStr(&u8g2, 77, 43, "Pa");
}

void InFlow(float value)
{
    uint8_t buff[10];
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
    u8g2_DrawStr(&u8g2, 2, 52, "InFlow:");
    if(value > 999)
        value = 999;
    sprintf(buff, "%02.1f", (value*0.8/480));
    u8g2_DrawStr(&u8g2, 53, 52, buff);
    u8g2_DrawStr(&u8g2, 77, 52, "m/s");
}

void DownFlow(float value)
{
    uint8_t buff[10];
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
    u8g2_DrawStr(&u8g2, 2, 61, "D.Flow:");
    if(value > 999)
        value = 999;
    sprintf(buff, "%02.1f", (value*0.8/480));
    u8g2_DrawStr(&u8g2, 53, 61, buff);
    u8g2_DrawStr(&u8g2, 77, 61, "m/s");
}

void Door(uint8_t DStatus)
{
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mf);
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mf);
    u8g2_DrawStr(&u8g2, 108, 61, "Door");
    u8g2_DrawHVLine(&u8g2, 109, 40, 13, 1);
    u8g2_DrawHVLine(&u8g2, 125, 40, 13, 1);
    u8g2_DrawHLine(&u8g2, 110, 40, 16);
    u8g2_DrawBox(&u8g2, 113, 50, 9, 3); //Close the door at level 1
    u8g2_DrawBox(&u8g2, 113, 46, 9, 3); //Close the door at level 2
    u8g2_DrawBox(&u8g2, 113, 42, 9, 3); //Close the door at level 3
    switch (DStatus)
    {
        case 0:
            u8g2_DrawBox(&u8g2, 113, 50, 9, 3); //Close the door at level 1
            u8g2_DrawBox(&u8g2, 113, 46, 9, 3); //Close the door at level 2
            u8g2_DrawBox(&u8g2, 113, 42, 9, 3); //Close the door at level 3
            break;
        case 1:
            u8g2_DrawStr(&u8g2, 113, 56, "   "); //Open the door at level 1
            break;
        case 2:
            u8g2_DrawStr(&u8g2, 113, 52, "   "); //Open the door at level 2
            break;
        case 3:
            u8g2_DrawStr(&u8g2, 113, 48, "   "); //Open the door at level 3
            u8g2_DrawStr(&u8g2, 113, 56, "   ");
            break;
        default:
            break;
    }
}

void Temperature(float temp)
{
    uint8_t buff[10];
    u8g2_DrawBitmap(&u8g2, 90, 22, bmp_temp.width / 8, bmp_temp.height, bmp_temp.data);
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
    u8g2_DrawStr(&u8g2, 110, 36, "C");
    sprintf(buff, "%02.1f", temp);
    u8g2_DrawStr(&u8g2, 105, 28, buff);
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 104, 44, 0x00b0);
}

void Info_Screen_Manage(void)
{
    RTC_Time(localTime);
    ExhaustFilter(sm5852_1.pressure);
    DownflowFilter(sm5852_2.pressure);
    InFlow(sm5852_1.pressure);
    DownFlow(sm5852_2.pressure);
    Temperature(sm5852_1.temp);
    Door(dev.doorLevel);
}
