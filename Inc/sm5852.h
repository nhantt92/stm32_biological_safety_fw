#ifndef __SM5852_H_
#define __SM5852_H_

#include "main.h"

#define SM5852_ADDR	0x5F

#define P_MIN 0.0f;
#define P_MAX 0.15f;

#define PRESSURE_MAX	480			//Measure max 480Pa
#define FLOW			0.8			//480Pa <=> 0.8 m/s

#define CORRECTED_PRESSURE_LSB 	 	128
#define CORRECTED_PRESSURE_MSB 	 	129

#define TEMPERATURE_LSB	 			130
#define TEMPERATURE_MSB				131

#define UNCORRECTED_PRESSURE_LSB	132
#define UNCORRECTED_PRESSURE_MSB	133

#define BAND_GAP_VOLTAGE_LSB		134
#define BAND_GAP_VOLTAGE_MSB		135

#define DigoutPmin  2045.0
#define DigoutPmax  3686.0
#define DigoutTmin  2045.0
#define DigoutTmax  3686.0

#define SS_SAMPLE 10; // read num sample, caculator avarage

typedef struct
{
	// uint8_t	state; //non-blocking filter average
	uint8_t buff[4];
	uint32_t tick;
	float pressure;
	float temp;
	I2C_HandleTypeDef i2cHandle;
} SM5852_T;

extern SM5852_T sm5852_1;
extern SM5852_T sm5852_2;

void SM5852_Init(SM5852_T *sm5852);
void SM5852_GetData(SM5852_T *sm5852);
void SM5852_Manager(SM5852_T *sm5852);

#endif