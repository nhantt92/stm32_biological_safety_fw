#ifndef __MAIN_SCREEN_H__
#define __MAIN_SCREEN_H__

#include "main.h"
#include "user_bitmap.h"

#define Minute_1            (1*60000)   //Test
#define Minute_15           (15*60000)  //Timer off UV 15'
#define Minute_30           (30*60000)  //Timer off UV 30'
#define Minute_60           (60*60000)  //Timer off UV 60'

typedef struct{
    uint32_t t_uv;
    uint32_t rotate_fan;
    uint32_t off_uv;
    uint32_t t_fan;
} DEVICE_TIMER;

typedef struct {
    uint8_t lampStatus;
    uint8_t uvStatus;
    uint8_t fanStatus;
    uint8_t socketStatus;
    uint8_t fanRotate;
    DEVICE_TIMER tick;
} MAIN_SCREEN_STR;

extern MAIN_SCREEN_STR main_scr;

void main_screen_init(void);
void Lamp_Status(uint8_t lampStatus);
void UV_Status(uint8_t uvStatus);
void TimerOff_Uv(uint32_t t_Uv);
void Fan_Status(uint8_t fanStatus);
void Socket_Status(uint8_t sStatus);
void Filter_Val(uint16_t val);
// void Filter_Pcent(uint8_t percent);
void Horizontal(void);
void Main_Screen_Manage(void);

#endif