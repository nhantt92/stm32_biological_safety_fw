#include "config.h"
#include <string.h>
#include <stdio.h>

CFG_STRUCT sys_cfg;

#define ADDR_FLASH_PAGE_188 	((uint32_t)0x0805E000)
#define ADDR_FLASH_PAGE_189		((uint32_t)0x0805E800)

#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_188   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_189 + FLASH_PAGE_SIZE   /* End @ of user Flash area */

#define DATA_32                 ((uint32_t)0x12345678)

uint32_t Address = 0, PAGEError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

void CFG_Save(void)
{
	uint32_t *cfgdest, i;
	sys_cfg.systemTime = sys;
	cfgdest = (uint32_t*)&sys_cfg;
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  	EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
  	EraseInitStruct.NbPages     = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;
	
	// printf("sizeof cfg: %d\n", sizeof(CFG_STRUCT));
	// for(i = 0; i <= sizeof(CFG_STRUCT); i+=4)
	// {
	// 	printf("%d: 0x%.8x\n", i, *(uint32_t*)(cfgdest + i/4));
	// }
	for(i = 0; i <= sizeof(CFG_STRUCT)/PAGE_SIZE; i++)
	{
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	  	{
	  	}
	}
	Address = FLASH_USER_START_ADDR;
	for(i = 0; i <= sizeof(CFG_STRUCT); i+=4)
	{
		//printf("%d: 0x%.8x\n", i, *(uint32_t*)(cfgdest + i/4));
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address + i, *(uint32_t*)(cfgdest + i/4)) == HAL_OK);
		if(*(uint32_t*)(cfgdest + i/4) != *(uint32_t*)(Address + i))
			return;
	}
	HAL_FLASH_Lock();
}

void CFG_Load(void)
{
	memcpy(&sys_cfg, (void*)FLASH_USER_START_ADDR, sizeof(CFG_STRUCT));
	// printf("modeUV: %d\n", sys_cfg.uvMode);
	// printf("viewPage: %d\n", sys_cfg.viewPage);
	// printf("brightness: %d\n", sys_cfg.brightness);
	// printf("backlight: %d\n", sys_cfg.backlight);
	// printf("speaker: %d\n", sys_cfg.speaker);
	// printf("pressure: %d\n", sys_cfg.pressureVal);
	// printf("passWord: %d\n", sys_cfg.passWord);
	// printf("tick: %d\n", sys_cfg.systemTime.tick);
	// printf("UVTime: %d\n", sys_cfg.systemTime.UVTime);
	// printf("FilterTime: %d\n", sys_cfg.systemTime.FilterTime);
	// printf("SysTime: %d\n", sys_cfg.systemTime.SysTime);
	if(sys_cfg.cfg_holder != CFG_HOLDER)
	{
		memset(&sys_cfg, 0x00, sizeof sys_cfg);
		sys_cfg.uvMode = 0;
		sys_cfg.viewPage = 0;
		sys_cfg.brightness = 50;
		sys_cfg.backlight = 1;

		sys_cfg.speaker = 1;
		sys_cfg.pressureVal = 300;
		sys_cfg.passWord = 1234;
		sys_cfg.cfg_holder = 0x00FF55A1;

		sys_cfg.systemTime.tick = 0;
		sys_cfg.systemTime.UVTime = 0;
		sys_cfg.systemTime.FilterTime = 0;
		sys_cfg.systemTime.SysTime = 0;

		CFG_Save();
	}
}
