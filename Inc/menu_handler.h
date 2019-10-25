#ifndef __MENU_HANDLER_H
#define __MENU_HANDLER_H

#include "main.h"

#define HOUR	3600 //(seconds)

uint16_t normalEvt(uint8_t add, uint16_t key);
uint16_t Dis1Evt(uint8_t add, uint16_t key);
uint16_t Dis2Evt(uint8_t add, uint16_t key);
uint16_t Dis3Evt(uint8_t add, uint16_t key);
uint16_t Dis4Evt(uint8_t add, uint16_t key);
uint16_t SetTimeEvt(uint8_t add, uint16_t key);
uint16_t SetUVEvt(uint8_t add, uint16_t key);
uint16_t SetPressureEvt(uint8_t add, uint16_t key);
uint16_t SetDisplayEvt(uint8_t add, uint16_t key);
uint16_t SetBrighnessEvt(uint8_t add, uint16_t key);
uint16_t SetBackLightEvt(uint8_t add, uint16_t key);
uint16_t ViewUVTimeEvt(uint8_t add, uint16_t key);
uint16_t ViewFilterTimeEvt(uint8_t add, uint16_t key);
uint16_t ViewSystemTimeEvt(uint8_t add, uint16_t key);
uint16_t ResetUVTimeEvt(uint8_t add, uint16_t key);
uint16_t ResetFilterTimeEvt(uint8_t add, uint16_t key);
uint16_t ResetSystemTimeEvt(uint8_t add, uint16_t key);
uint16_t SetSpeakerEvt(uint8_t add, uint16_t key);
#endif
