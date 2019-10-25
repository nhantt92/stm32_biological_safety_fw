#ifndef __MAIN_SCREEN_H__
#define __MAIN_SCREEN_H__

#include "main.h"
#include "user_bitmap.h"

#define F_WARNING 5

typedef struct {
    uint8_t fanRotate;
    uint8_t warnCnt;
    uint32_t tick;
} MAIN_SCREEN_T;

extern MAIN_SCREEN_T main_scr;

void main_screen_init(void);
void Lamp_Status(uint8_t lampStatus);
void UV_Status(uint8_t uvStatus);
void TimerOff_Uv(uint32_t t_Uv);
void Fan_Status(uint8_t fanStatus);
void Socket_Status(uint8_t sStatus);
// void Filter_Val(uint16_t val);
void Filter_Pcent(float pressure, uint16_t threshold);
void Horizontal(void);
void Main_Screen_Manage(void);

#endif