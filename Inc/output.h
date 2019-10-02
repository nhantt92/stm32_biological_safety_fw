#ifndef __OUTPUT_H
#define __OUTPUT_H

#include "main.h"

void Output_Init(void);
void Output_Manage(void);
void SW_Neon(uint8_t neonStatus);
void SW_Uv(uint8_t uvStatus);
void SW_Fan(uint8_t fanStatus);
void SW_Socket(uint8_t socketStatus);
#endif