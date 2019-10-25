#include "system.h"
#include "config.h"
#include "clock_rtc.h"
#include "main_screen.h"
#include "info_screen.h"
#include <string.h>
#include <stdio.h>
#include "menu.h"

SYS_STRUCT_T sys;
DEVICE_T dev;

uint8_t toggle = 0;

static void Save_UVTime_BKUP(void);
static void Load_UVTime_BKUP(void);
static void Load_SysTime(void);

void System_Init(void)
{
	CFG_Load();
	Device_Load_Status();
	Load_UVTime_BKUP();
	Load_SysTime();
	dev.doorLevel = 0;
	Set_Backlight(sys_cfg.brightness);
	sys_cfg.backlight?BL_On():BL_Off();
	main_screen_init();
	sys.tick = HAL_GetTick();
}

void Device_Save_Status(void)
{
	uint16_t *savedest;
	savedest = (uint16_t*)&dev.status;
	// printf("padding: 0x%.4x\n", *(uint16_t*)(savedest));
	// printf("padding: 0x%.4x\n", *(uint16_t*)(savedest+1));
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, *(uint16_t*)(savedest));
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, *(uint16_t*)(savedest+1)); 
	//printf("padding: 0x%.8x\n", *(uint32_t*)(savedest));
}

void Device_Load_Status(void)
{
	uint32_t tmp;
	tmp = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2));
	memcpy(&dev.status, &tmp, 4);
	// printf("tmp: 0x%.8x\n", tmp);
	// printf("L: %d\n", dev.status.lamp);
	// printf("Ne: %d\n", dev.status.uv);
	// printf("F: %d\n", dev.status.fan);
	// printf("S: %d\n", dev.status.socket);
	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR6) != DEVICE_HOLDER) 
    {
        dev.status.lamp = 0;
        dev.status.uv = 0;
        dev.status.fan = 0;
        dev.status.socket = 0;
        Device_Save_Status();
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR6, DEVICE_HOLDER);
    }
}

void UV_Init_Time(uint8_t mode)
{
	switch(mode)
	{
		case 1:			/* AUTO TURN OFF AFTER 15 MINUTES*/
			dev.UVFlag = 1;
			dev.UVTimeOff = 15*60;
			break;
		case 2:			/* AUTO TURN OFF AFTER 30 MINUTES*/
			dev.UVFlag = 1;
			dev.UVTimeOff = 30*60;
			break;
		case 3:			/* AUTO TURN OFF AFTER 60 MINUTES*/
			dev.UVFlag = 1;
			dev.UVTimeOff = 60*60;
			break;		
		case 4:			/* AUTO TURN OFF AFTER 120 MINUTES*/
			dev.UVFlag = 1;
			dev.UVTimeOff = 2*60*60;
			break;		
		default: 		/* MANUAL TURN OFF*/
			dev.UVFlag = 0;
			dev.UVTimeOff = 0;
			break;
	}
	//printf("Set: FlagUV: %d, TimeUVOff: %d\n", dev.UVFlag, dev.UVTimeOff);				
}

void UV_Clear_Time(void)
{
	dev.UVFlag = 0;
	dev.UVTimeOff = 0;
	Save_UVTime_BKUP();
	//printf("Clear: FlagUV: %d, TimeUVOff: %d\n", dev.UVFlag, dev.UVTimeOff);				
}


void UV_Check_Time(uint32_t time)
{
	if((dev.UVFlag) && (time == 0))
	{
		//dev.UVFlag = 0;
		dev.status.uv = 0;
		//sys.UVTime = sys.UVTime + dev.UVTimeOff;
		UV_Clear_Time();
		Device_Save_Status();
	}
	else
		dev.status.uv = dev.status.uv;
}

void System_Manager(void)
{
	if(HAL_GetTick() - sys.tick >= 1000) //Every seconds
	{
		if(++toggle > TIME_PAGE*2) toggle = 0;
		if(++sys.SysTime > SYSTEM_TIME_MAX)
			sys.SysTime = SYSTEM_TIME_MAX;
		//printf("System Time: %d s\n", sys.SysTime);
		if(dev.status.fan) 
			sys.FilterTime++;
		//printf("FilterTime: %d s\n", sys.FilterTime);
		if(dev.UVFlag)
		{	
			if(dev.status.uv) sys.UVTime++;
			if(--dev.UVTimeOff <= 0)
				dev.UVTimeOff = 0;
			//printf("UVTime: %d s\n", sys.UVTime);
			Save_UVTime_BKUP();
		}
		sys.tick = HAL_GetTick();
		//printf("FlagUV: %d, TimeUVOff: %d, status: %d\n", dev.UVFlag, dev.UVTimeOff, dev.status.uv);
		Save_SysTime_BKUP();
		// printf("UVTime: %d \n", sys.UVTime);
		// printf("FilterTime: %d \n", sys.FilterTime);
		// printf("SysTime: %d \n", sys.SysTime);

		/* Test save Time to BKUP */
		// printf("BKtick: %d\n", (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR12) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR11)));
		// printf("BKUV: %d\n", (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR14) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR13)));
		// printf("BKFil: %d\n", (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR16) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR15)));
		// printf("BKSys: %d\n", (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR18) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR17)));
		// printf("_______\n");
		// printf("Ftick: %d \n", sys_cfg.systemTime.tick);
		// printf("FUV: %d \n", sys_cfg.systemTime.UVTime);
		// printf("FFil: %d \n", sys_cfg.systemTime.FilterTime);
		// printf("FSys: %d \n", sys_cfg.systemTime.SysTime);
		// printf("_______\n");
	}
	if(menuIdx == 0)
	{

		UV_Check_Time(dev.UVTimeOff);
		if(sys_cfg.viewPage == 1)
	    {
	        u8g2_ClearBuffer(&u8g2);
	        Main_Screen_Manage();
	    }
	    else if(sys_cfg.viewPage == 2)
	    {
	        u8g2_ClearBuffer(&u8g2);
	        Info_Screen_Manage();
	    }
	    else
	    {
	        if(toggle >= TIME_PAGE)
	        {
	          u8g2_ClearBuffer(&u8g2);
	          Info_Screen_Manage();
	        }
	        else
	        {
	          u8g2_ClearBuffer(&u8g2);
	          Main_Screen_Manage();
	        }
	    }
	}
}

static void Save_UVTime_BKUP(void)
{
	uint16_t *savedest;
	savedest = (uint16_t*)&dev.UVTimeOff;
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, dev.UVFlag);
	/* Save Time 32bit TimeUVOff, value use 16bit */
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, *(uint16_t*)(savedest)); 	
	//HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR6, *(uint16_t*)(savedest + 1)); 
	//printf("padding: 0x%.8x\n", *(uint32_t*)(savedest));
}

static void Load_UVTime_BKUP(void)
{
	dev.UVFlag = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
	dev.UVTimeOff = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR6) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5));
	// printf("UVFlag: %d\n", dev.UVFlag);
	// printf("UVTimeOff: %d\n", dev.UVTimeOff);
	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != RTC_HOLDER) 
    {
        dev.UVFlag = 0;
        dev.UVTimeOff = 0;
        Save_UVTime_BKUP();
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, RTC_HOLDER);
    }
}

void Save_SysTime_BKUP(void)
{
	uint32_t tmp = 0U;
	uint16_t i, *savedest;
	savedest = (uint16_t*)&sys;
	for(i = 0; i < sizeof(SYS_STRUCT_T)/2; i++)
	{ 
		// printf("savedest %d :  %d\n", i, *(uint16_t*)(savedest + i));
		tmp = (uint32_t)BKP_BASE; 
		tmp += ((RTC_BKP_DR11 + i)* 4U);
		*(__IO uint32_t *) tmp = (*(uint16_t*)(savedest + i) & BKP_DR1_D);
	}
}

static void Load_SysTime(void)
{
	sys.tick = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR12) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR11));
	sys.UVTime = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR14) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR13));
	sys.FilterTime = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR16) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR15));
	sys.SysTime = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR18) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR17));
	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR19) != TIME_HOLDER) 
    {
    	// Read from Flash
        sys = sys_cfg.systemTime;
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR19, TIME_HOLDER);
    }
}