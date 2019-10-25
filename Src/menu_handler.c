#include "menu.h"
#include "menu_handler.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "clock_rtc.h"
#include "info_screen.h"
#include "system.h"
#include "lcd_backlight.h"
#include "config.h"

uint8_t buff[90];
uint8_t ten,unit;
uint8_t logGetBuff[32];
DATE_TIME menuRtc;

uint16_t normalEvt(uint8_t add, uint16_t key)
{
  return 0;
}

uint16_t Dis1Evt(uint8_t add, uint16_t key)
{
  menuRtc = localTime;
  return 0;
}

uint16_t Dis2Evt(uint8_t add, uint16_t key)
{
  return 0;
}

uint16_t Dis3Evt(uint8_t add, uint16_t key)
{
  return 0;
}

uint16_t Dis4Evt(uint8_t add, uint16_t key)
{
  return 0;
}

uint16_t SetTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuCursor = 0;
  }
  if(key == BTN_NEXT)
  {
    if(++menuCursor>5) menuCursor = 0;
    // if(menuCursor < 5)menuCursor++;
  }

  if(key == BTN_UP)
  {
    switch(menuCursor)
    {
      case 0:
        if(menuRtc.hour < 23)menuRtc.hour++;
        break;
      case 1:
        if(menuRtc.min < 59)menuRtc.min++;
        break;
      case 2:
        if(menuRtc.sec < 59)menuRtc.sec++;
        break;
      case 3:
        if(menuRtc.mday < 31)menuRtc.mday++;
        break;
      case 4:
        if(menuRtc.month < 12)menuRtc.month++;
        break;
      case 5:
        if(menuRtc.year < 2099)menuRtc.year++;
        break;
    }
  }
  if(key == BTN_DOWN)
  {
    switch(menuCursor)
    {
      case 0:
        if(menuRtc.hour != 0)menuRtc.hour--;
        break;
      case 1:
        if(menuRtc.min != 0)menuRtc.min--;
        break;
      case 2:
        if(menuRtc.sec != 0)menuRtc.sec--;
        break;
      case 3:
        if(menuRtc.mday > 1)menuRtc.mday--;
        break;
      case 4:
        if(menuRtc.month > 1)menuRtc.month--;
        break;
      case 5:
        if(menuRtc.year > 2000)menuRtc.year--;
        break;
    }
  }
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 5, 36, 0x23f1);
  switch(menuCursor)
  {
    case 0:
      /* blink hours */
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%02d", menuRtc.hour);
      u8g2_DrawStr(&u8g2, 36, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, ":%02d:%02d", menuRtc.min, menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36 + 12, 36, buff);
      sprintf((char*)buff,"%02d/%02d/%04d", menuRtc.mday, menuRtc.month, menuRtc.year);
      u8g2_DrawStr(&u8g2, 30, 48, buff);

      break;
    case 1:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%02d:", menuRtc.hour);
      u8g2_DrawStr(&u8g2, 36, 36, buff);
      /* blink minutes */
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%02d", menuRtc.min);
      u8g2_DrawStr(&u8g2, 36 + 18, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, ":%02d", menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36 + 30, 36, buff);

      sprintf((char*)buff,"%02d/%02d/%04d", menuRtc.mday, menuRtc.month, menuRtc.year);
      u8g2_DrawStr(&u8g2, 30, 48, buff);
      break;
    case 2:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%02d:%02d:", menuRtc.hour, menuRtc.min);
      u8g2_DrawStr(&u8g2, 36, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%02d", menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36 + 36, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%02d/%02d/%04d", menuRtc.mday, menuRtc.month, menuRtc.year);
      u8g2_DrawStr(&u8g2, 30, 48, buff);
      break;
    case 3:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%02d:%02d:%02d", menuRtc.hour, menuRtc.min, menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%02d", menuRtc.mday);
      u8g2_DrawStr(&u8g2, 30, 48, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "/%02d/%04d", menuRtc.month, menuRtc.year);
      u8g2_DrawStr(&u8g2, 30 + 12, 48, buff);
      break;
    case 4:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%02d:%02d:%02d", menuRtc.hour, menuRtc.min, menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36, 36, buff);

      sprintf((char*)buff, "%02d/", menuRtc.mday);
      u8g2_DrawStr(&u8g2, 30, 48, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%02d", menuRtc.month);
      u8g2_DrawStr(&u8g2, 30+18, 48, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "/%04d", menuRtc.year);
      u8g2_DrawStr(&u8g2, 30 + 30, 48, buff);
      break;
    case 5:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%02d:%02d:%02d", menuRtc.hour, menuRtc.min, menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36, 36, buff);

      sprintf((char*)buff, "%02d/%02d/", menuRtc.mday, menuRtc.month);
      u8g2_DrawStr(&u8g2, 30, 48, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%04d", menuRtc.year);
      u8g2_DrawStr(&u8g2, 30 + 36, 48, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      break;
  }
  if(key == BTN_MENU)
  {
    localTime = menuRtc;
    localTime.hour = localTime.hour - 7;
    RTC_Sync(&localTime, +7);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return treeView[add].keyMode;
}


uint16_t SetUVEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = sys_cfg.uvMode;
  if(key == BTN_UP)
    if(menuValueSet < 4)
      menuValueSet++;
  if(key == BTN_DOWN)
    if(menuValueSet > 0)
      menuValueSet--;
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  //u8g2_DrawStr(&u8g2, ((22 - strlen("^"))/2)*6, 24, "^");
  u8g2_DrawGlyph(&u8g2, 56, 24, 0x23f6);
  if(menuValueSet == 0)
    sprintf((char*)buff, "OFF");
  if(menuValueSet == 1)
    sprintf((char*)buff, "15 MINUTE");
  if(menuValueSet == 2)
    sprintf((char*)buff, "30 MINUTE");
  if(menuValueSet == 3)
    sprintf((char*)buff, "1 HOURS");
  if(menuValueSet == 4)
    sprintf((char*)buff, "2 HOURS");
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 56, 50, 0x23f7);
  //u8g2_DrawStr(&u8g2, ((22 - strlen("v"))/2)*6, 48, "v");
  if(key == BTN_MENU)
  {
    sys_cfg.uvMode = (uint8_t)menuValueSet;
    CFG_Save();
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return treeView[add].keyMode;
}

uint16_t SetPressureEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = sys_cfg.pressureVal;
  if(key == BTN_UP)
    if(menuValueSet < 800)
      menuValueSet++;
  if(key == BTN_DOWN)
    if(menuValueSet > 0)
      menuValueSet--;
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 35, 38, 0x23f2);

  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 56, 24, 0x23f6);

  sprintf((char*)buff, "%0000d", menuValueSet);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);

  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 56, 50, 0x23f7);
  if(key == BTN_MENU)
  {
    sys_cfg.pressureVal = menuValueSet;
    CFG_Save(); // Save config
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return 0;
}

uint16_t SetDisplayEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
     menuValueSet = sys_cfg.viewPage;
  if(key == BTN_UP)
    if(menuValueSet < 2)
      menuValueSet++;
  if(key == BTN_DOWN)
    if(menuValueSet > 0)
      menuValueSet--;

  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 56, 24, 0x23f7);

  if(menuValueSet == 0)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 20, 36, 0x25a3);
    sprintf((char*)buff, "AUTO");
  }
  if(menuValueSet == 1)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 20, 36, 0x25a4);
    sprintf((char*)buff, "MAIN PAGE");
  }
  if(menuValueSet == 2)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 20, 36, 0x25a5);
    sprintf((char*)buff, "INFO PAGE");
  }

  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 56, 50, 0x23f7);
  if(key == BTN_MENU)
  {
    sys_cfg.viewPage = menuValueSet;
    CFG_Save();
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return 0;
}

uint16_t SetBrighnessEvt(uint8_t add, uint16_t key)
{
  printf("Key pointerFunc: %d\n", key);
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = sys_cfg.brightness;
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 5, 36, 0x2600);
  if(key == BTN_UP)
    if(menuValueSet < 100)
      menuValueSet++;
  if(key == BTN_DOWN)
    if(menuValueSet > 0)
      menuValueSet--;
  Set_Backlight((uint8_t)menuValueSet);
  
  u8g2_DrawBox(&u8g2, 20, 24, menuValueSet, 10); 
  
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 40, 48, 0x23eb);
  u8g2_DrawGlyph(&u8g2, 77, 48, 0x23ec);

  sprintf((char*)buff, "%03d%%", menuValueSet);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 48, buff);

  if(key == BTN_MENU)
  {
    sys_cfg.brightness = (uint8_t)menuValueSet;
    CFG_Save();
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return 0;
}

uint16_t SetBackLightEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = sys_cfg.backlight;
  if(key == BTN_UP)
    if(menuValueSet < 1)
      menuValueSet++;
  if(key == BTN_DOWN)
    if(menuValueSet > 0)
      menuValueSet--;

  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 56, 24, 0x23f6);

  if(menuValueSet == 0)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 35, 36, 0x2610);
    sprintf((char*)buff, "OFF");
    BL_Off();
  }
  if(menuValueSet == 1)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 35, 36, 0x2611);
    sprintf((char*)buff, "ON");
    BL_On();
  }
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 56, 50, 0x23f7);
  if(key == BTN_MENU)
  {
    sys_cfg.backlight = (uint8_t)menuValueSet;
    CFG_Save();
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return 0;
}

uint16_t ViewUVTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = 0;
  // if(key == BTN_UP)
  //   if(menuValueSet < 50)
  //     menuValueSet++;
  // if(key == BTN_DOWN)
  //   if(menuValueSet > 1)
  //     menuValueSet--;
  if(sys.UVTime/HOUR > 9999) sys.UVTime = sys.UVTime * HOUR;
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 55, 54, 0x23f0);
  sprintf((char*)buff, "%04d Hours", sys.UVTime/HOUR);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen("UV Time Worked!"))/2)*6, 24, "UV Time Worked!");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
 
  // if(key == BTN_MENU)
  // {
      // CFG_Save();
      // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      // u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  // }
  return 0;
}

uint16_t ViewFilterTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = 0;
  // if(key == BTN_UP)
  //   if(menuValueSet < 50)
  //     menuValueSet++;
  // if(key == BTN_DOWN)
  //   if(menuValueSet > 1)
  //     menuValueSet--;
  if(sys.FilterTime/HOUR > 9999) sys.FilterTime = sys.FilterTime * HOUR;
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 55, 54, 0x23f0);
  sprintf((char*)buff, "%04d Hours", sys.FilterTime/HOUR);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen("Filter Time Worked!"))/2)*6, 24, "Filter Time Worked!");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
 
  // if(key == BTN_MENU)
  // {
      // CFG_Save();
      // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      // u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  // }
  return 0;
}

uint16_t ViewSystemTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = 0;
  // if(key == BTN_UP)
  //   if(menuValueSet < 50)
  //     menuValueSet++;
  // if(key == BTN_DOWN)
  //   if(menuValueSet > 1)
  //     menuValueSet--;
  if(sys.SysTime/HOUR > 99999) sys.SysTime = sys.SysTime * HOUR;
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 55, 54, 0x23f0);
  sprintf((char*)buff, "%05d Hours", sys.SysTime/HOUR);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen("Systems Time Worked!"))/2)*6, 24, "Systems Time Worked!");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
 
  // if(key == BTN_MENU)
  // {
      // CFG_Save();
      // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      // u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  // }
  return 0;
}

uint8_t nghin, tram, chuc, dv;

uint16_t ResetUVTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuValueSet = 0;
    menuCursor = 0;
    nghin = tram = chuc = dv = 0;
  }
  if(key == BTN_NEXT)
  {
    if(++menuCursor > 3) menuCursor = 0;
  }

  if(key == BTN_UP)
  {
    switch(menuCursor)
    {
      case 0:
        if(nghin < 9) nghin++;
        break;
      case 1:
        if(tram < 9) tram++;
        break;
      case 2:
        if(chuc < 9) chuc++;
        break;
      case 3:
        if(dv < 9) dv++;
        break;
    }
  }
  if(key == BTN_DOWN)
  {
    switch(menuCursor)
    {
      case 0:
        if(nghin > 0) nghin--;
        break;
      case 1:
        if(tram > 0) tram--;
        break;
      case 2:
        if(chuc > 0) chuc--;
        break;
      case 3:
        if(dv > 0) dv--;
        break;
    }
  }
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2,((22 - strlen("Enter Password!"))/2)*6, 24, "Enter Password!");
  switch(menuCursor)
  {
    case 0:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", nghin);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d%d", tram, chuc, dv);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      break;
    case 1:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d", nghin);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      /* blink minutes */
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", tram);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d", chuc, dv);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      break;
    case 2:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d", nghin, tram);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", chuc);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      sprintf((char*)buff, "%d", dv);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      break;
    case 3:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d%d", nghin, tram, chuc);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", dv);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      break;
  }
  // sprintf((char*)buff, "%04d", menuValueSet);
  // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  // u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
  menuValueSet = nghin*1000 + tram*100 + chuc*10 + dv;
  if(menuValueSet == sys_cfg.passWord)
  {
    if(key == BTN_MENU)
    {
      sys.UVTime = 0;
      Save_SysTime_BKUP();
      CFG_Save();
      sprintf((char*)buff, "%04d Hours", sys.UVTime);
      u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
      sprintf((char*)buff, "\4 Reset UV Time!\n");
      u8g2_DrawStr(&u8g2,((22 - strlen("Reset UV Time!"))/2)*6, 48, "Reset UV Time!");
    }
  }
  else
  {
    if(key == BTN_MENU)
    {
      sys.UVTime = sys.UVTime;
      sprintf((char*)buff, "%04d Hours", sys.UVTime);
      u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
      u8g2_DrawStr(&u8g2,((22 - strlen("Wrong Password!"))/2)*6, 48, "Wrong Password!");
    }
  }
  return 0;
}

uint16_t ResetFilterTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuValueSet = 0;
    menuCursor = 0;
    nghin = tram = chuc = dv = 0;
  }
  if(key == BTN_NEXT)
  {
    if(++menuCursor > 3) menuCursor = 0;
  }

  if(key == BTN_UP)
  {
    switch(menuCursor)
    {
      case 0:
        if(nghin < 9) nghin++;
        break;
      case 1:
        if(tram < 9) tram++;
        break;
      case 2:
        if(chuc < 9) chuc++;
        break;
      case 3:
        if(dv < 9) dv++;
        break;
    }
  }
  if(key == BTN_DOWN)
  {
    switch(menuCursor)
    {
      case 0:
        if(nghin > 0) nghin--;
        break;
      case 1:
        if(tram > 0) tram--;
        break;
      case 2:
        if(chuc > 0) chuc--;
        break;
      case 3:
        if(dv > 0) dv--;
        break;
    }
  }
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2,((22 - strlen("Enter Password!"))/2)*6, 24, "Enter Password!");
  switch(menuCursor)
  {
    case 0:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", nghin);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d%d", tram, chuc, dv);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      break;
    case 1:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d", nghin);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      /* blink minutes */
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", tram);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d", chuc, dv);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      break;
    case 2:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d", nghin, tram);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", chuc);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      sprintf((char*)buff, "%d", dv);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      break;
    case 3:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d%d", nghin, tram, chuc);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", dv);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      break;
  }
  // sprintf((char*)buff, "%04d", menuValueSet);
  // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  // u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
  menuValueSet = nghin*1000 + tram*100 + chuc*10 + dv;
  if(menuValueSet == sys_cfg.passWord)
  {
    if(key == BTN_MENU)
    {
      sys.FilterTime = 0;
      Save_SysTime_BKUP();
      CFG_Save();
      sprintf((char*)buff, "%04d Hours", sys.FilterTime); 
      u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
      sprintf((char*)buff, "\4 Reset UV Time!\n");
      u8g2_DrawStr(&u8g2,((22 - strlen("Reset UV Time!"))/2)*6, 48, "Reset UV Time!");
    }
  }
  else
  {
    if(key == BTN_MENU)
    {
      sys.FilterTime = sys.FilterTime;
      sprintf((char*)buff, "%04d Hours", sys.FilterTime);
      u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
      u8g2_DrawStr(&u8g2,((22 - strlen("Wrong Password!"))/2)*6, 48, "Wrong Password!");
    }
  }
  return 0;
}

uint16_t ResetSystemTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuValueSet = 0;
    menuCursor = 0;
    nghin = tram = chuc = dv = 0;
  }
  if(key == BTN_NEXT)
  {
    if(++menuCursor > 3) menuCursor = 0;
  }

  if(key == BTN_UP)
  {
    switch(menuCursor)
    {
      case 0:
        if(nghin < 9) nghin++;
        break;
      case 1:
        if(tram < 9) tram++;
        break;
      case 2:
        if(chuc < 9) chuc++;
        break;
      case 3:
        if(dv < 9) dv++;
        break;
    }
  }
  if(key == BTN_DOWN)
  {
    switch(menuCursor)
    {
      case 0:
        if(nghin > 0) nghin--;
        break;
      case 1:
        if(tram > 0) tram--;
        break;
      case 2:
        if(chuc > 0) chuc--;
        break;
      case 3:
        if(dv > 0) dv--;
        break;
    }
  }
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2,((22 - strlen("Enter Password!"))/2)*6, 24, "Enter Password!");
  switch(menuCursor)
  {
    case 0:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", nghin);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d%d", tram, chuc, dv);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      break;
    case 1:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d", nghin);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      /* blink minutes */
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", tram);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d", chuc, dv);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      break;
    case 2:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d", nghin, tram);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", chuc);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      sprintf((char*)buff, "%d", dv);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      break;
    case 3:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d%d", nghin, tram, chuc);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", dv);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      break;
  }
  // sprintf((char*)buff, "%04d", menuValueSet);
  // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  // u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
  menuValueSet = nghin*1000 + tram*100 + chuc*10 + dv;
  if(menuValueSet == sys_cfg.passWord)
  {
    if(key == BTN_MENU)
    {
      sys.SysTime = 0;
      Save_SysTime_BKUP();
      CFG_Save();
      sprintf((char*)buff, "%05d Hours", sys.SysTime);
      u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
      sprintf((char*)buff, "\4 Reset UV Time!\n");
      u8g2_DrawStr(&u8g2,((22 - strlen("Reset UV Time!"))/2)*6, 48, "Reset UV Time!");
    }
  }
  else
  {
    if(key == BTN_MENU)
    {
      sys.SysTime = sys.SysTime;
      sprintf((char*)buff, "%05d Hours", sys.SysTime);
      u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
      u8g2_DrawStr(&u8g2,((22 - strlen("Wrong Password!"))/2)*6, 48, "Wrong Password!");
    }
  }
  return 0;
}

uint16_t SetSpeakerEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = sys_cfg.speaker;
  if(key == BTN_UP)
    if(menuValueSet < 1)
      menuValueSet++;
  if(key == BTN_DOWN)
    if(menuValueSet > 0)
      menuValueSet--;

  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 56, 24, 0x23f6);

  if(menuValueSet == 0)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 35, 36, 0x2610);
    sprintf((char*)buff, "OFF");
  }
  if(menuValueSet == 1)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 35, 36, 0x2611);
    sprintf((char*)buff, "ON");
  }
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 56, 50, 0x23f7);
  if(key == BTN_MENU)
  {
    sys_cfg.speaker = (uint8_t)menuValueSet;
    CFG_Save();
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return 0;
}

// uint16_t ViewLogEvt(uint8_t add, uint16_t key)
// {
//   memset(buff, 0, sizeof(buff));
//   if(treeView[add].keyMode != menuIdxLast)
//   {
//     menuCursor = 0;
//   }
//   if(key == BTN_UP)
//   {
//     if(menuCursor < 340)
//       menuCursor += 4;
//     else
//       menuCursor = 0;
//   }
//   if(key == BTN_DOWN)
//   {
//     if(menuCursor > 0)
//       menuCursor -= 4;
//     else
//       menuCursor = 335;
//   }
//   LOG_Read(LOG_ADDRESS_START+((menuCursor+0)*8), &logGetBuff[0]);
//   LOG_Read(LOG_ADDRESS_START+((menuCursor+1)*8), &logGetBuff[8]);
//   LOG_Read(LOG_ADDRESS_START+((menuCursor+2)*8), &logGetBuff[16]);
//   LOG_Read(LOG_ADDRESS_START+((menuCursor+3)*8), &logGetBuff[24]);
//   sprintf((char*)buff, "\1 %02d/%02d/%02d-%02d:%02d %02d.%d ", logGetBuff[0], logGetBuff[1], logGetBuff[2], logGetBuff[3], logGetBuff[4], (((uint16_t)logGetBuff[5]<<8)+logGetBuff[6])/10, (((uint16_t)logGetBuff[5]<<8)+logGetBuff[6])%10);
//   sprintf((char*)&buff[22], "\2 %02d/%02d/%02d-%02d:%02d %02d.%d ", logGetBuff[8], logGetBuff[9], logGetBuff[10], logGetBuff[11], logGetBuff[12], (((uint16_t)logGetBuff[13]<<8)+logGetBuff[14])/10, (((uint16_t)logGetBuff[13]<<8)+logGetBuff[14])%10);
//   sprintf((char*)&buff[44], "\3 %02d/%02d/%02d-%02d:%02d %02d.%d ", logGetBuff[16], logGetBuff[17], logGetBuff[18], logGetBuff[19], logGetBuff[20], (((uint16_t)logGetBuff[21]<<8)+logGetBuff[22])/10, (((uint16_t)logGetBuff[21]<<8)+logGetBuff[22])%10);
//   sprintf((char*)&buff[66], "\4 %02d/%02d/%02d-%02d:%02d %02d.%d \n", logGetBuff[24], logGetBuff[25], logGetBuff[26], logGetBuff[27], logGetBuff[28], (((uint16_t)logGetBuff[29]<<8)+logGetBuff[30])/10, (((uint16_t)logGetBuff[29]<<8)+logGetBuff[30])%10);
//   DISPLAY_Puts(buff);
//   return treeView[add].keyMode;
// }

// uint16_t SendLogEvt(uint8_t add, uint16_t key)
// {
//   LOG_Send();
//   return treeView[add].keyMode;
// }

