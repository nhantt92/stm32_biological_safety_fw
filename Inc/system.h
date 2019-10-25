#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "main.h"

#define SYSTEM_TIME_MAX		3600*24*365*100 /* 100 years (s) */
#define TIME_PAGE 			15

#define DEVICE_HOLDER		0xAAAA
#define TIME_HOLDER			0xBBBB

typedef struct
{
	uint8_t lamp;
	uint8_t uv;
	uint8_t fan;
	uint8_t socket;
} DEVICE_STATUS_T;

typedef struct
{
	uint8_t UVFlag;
	uint8_t doorLevel;
	DEVICE_STATUS_T status;
	uint32_t UVTimeOff;
} DEVICE_T;

typedef struct
{
	uint32_t tick;
	uint32_t UVTime;
	uint32_t FilterTime;
	uint32_t SysTime;
} SYS_STRUCT_T;

extern SYS_STRUCT_T sys;
extern DEVICE_T dev;
extern uint8_t toggle;

void System_Init(void);
void Device_Save_Status(void);
void Device_Load_Status(void);
void Save_SysTime_BKUP(void);
void System_Manager(void);
void UV_Init_Time(uint8_t mode);
void UV_Clear_Time(void);
#endif