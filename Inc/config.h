#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "main.h"
#include "system.h"

#define PAGE_SIZE			(0x400) /* 1 Kbyte*/
#define FLASH_SIZE			(0x20000) /* 128 Kbytes */
#define CONFIG_AREA_SIZE	4096
#define CONFIG_AREA_START	((uint32_t)((0x8000000 + FLASH_SIZE) - PAGE_SIZE*(CONFIG_AREA_SIZE/PAGE_SIZE)))

#define CFG_HOLDER	0x00FF55A1

typedef struct {
	uint8_t uvMode;
	uint8_t viewPage;
	uint8_t brightness;
	uint8_t backlight;
	uint8_t speaker;
	uint16_t pressureVal;
	uint16_t passWord;
	SYS_STRUCT_T systemTime;
	uint32_t cfg_holder;
} CFG_STRUCT;

extern CFG_STRUCT sys_cfg;

void CFG_Save(void);
void CFG_Load(void);

#endif