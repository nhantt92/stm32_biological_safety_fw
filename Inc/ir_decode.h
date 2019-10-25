#ifndef __IR_DECODE_H
#define __IR_DECODE_H

#include "main.h"

extern TIM_HandleTypeDef htim3;
const typedef enum
{
  POWER = 0x00fe,
  UP =   0x08f6,
  DOWN = 0x28d6,
  PREV = 0x8876,
  NEXT = 0xc836,
  PLAY = 0x48b6,
  LIST = 0x54aa,
  BACK = 0x708e,
  HOME = 0x946a,
} IRCommands;

/* NEC frame structure*/
typedef struct
{
	uint8_t startBit;
  	uint8_t cnt;
  	uint8_t cntTmp;
  	int8_t cntData;
  	uint32_t dataRev;
  	uint16_t addr;
  	uint16_t cmd;
} NEC_Frame_TypeDef;

extern NEC_Frame_TypeDef necFrame;
extern uint16_t IRCmd;

void NEC_Receiver_Init(void);
void NEC_MeasureFirstLowDuration(void);
void NEC_Decode(void);
void NEC_Manager(void);
#endif