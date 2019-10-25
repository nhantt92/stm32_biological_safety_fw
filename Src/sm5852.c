#include "sm5852.h"
#include "i2c.h"
#include <string.h>
#include "kalman_filter.h"
#include <stdio.h>
#include <stdlib.h>

static float toPressure(uint8_t hi, uint8_t lo);
static float toTemp(uint8_t hi, uint8_t lo);

void SM5852_Init(SM5852_T *sm5852)
{
	if(sm5852->i2cHandle.Instance == I2C1)
	{
		//printf("I2C1\n");
		MX_I2C1_Init();
		HAL_I2C_Master_Transmit(&hi2c1, SM5852_ADDR << 1, 0x00, 1, 100);
	}
	else if(sm5852->i2cHandle.Instance == I2C2)
	{
		//printf("I2C2\n");
		MX_I2C2_Init();
		HAL_I2C_Master_Transmit(&hi2c2, SM5852_ADDR << 1, 0x00, 1, 100);	
	}
	//sm5852->state = 0;  //only use when use average filter 
	memset(sm5852->buff, 0x00, sizeof(sm5852->buff));
	sm5852->pressure = 0;
	sm5852->temp = 0;
	Simple_Kalman_Filter(1, 1, 0.01);
	sm5852->tick = HAL_GetTick();
}

void SM5852_GetData(SM5852_T *sm5852)
{
	if(sm5852->i2cHandle.Instance == I2C1)
	{
		//printf("Read Sensor I2C1\n");
		HAL_I2C_Master_Transmit(&hi2c1, 0x5F << 1, 0x00, 1, 100);
		HAL_I2C_Mem_Read(&hi2c1, SM5852_ADDR << 1, CORRECTED_PRESSURE_LSB, 1, sm5852->buff, 1, 1000);
		HAL_I2C_Mem_Read(&hi2c1, SM5852_ADDR << 1, CORRECTED_PRESSURE_MSB, 1, sm5852->buff + 1, 1, 1000);
		HAL_I2C_Mem_Read(&hi2c1, SM5852_ADDR << 1, TEMPERATURE_LSB, 1, sm5852->buff + 2, 1, 1000);
		HAL_I2C_Mem_Read(&hi2c1, SM5852_ADDR << 1, TEMPERATURE_MSB, 1, sm5852->buff + 3, 1, 1000);
	}
	else if(sm5852->i2cHandle.Instance == I2C2)
	{
		//printf("Read Sensor I2C2\n");
		HAL_I2C_Master_Transmit(&hi2c1, 0x5F << 1, 0x00, 1, 100);
		HAL_I2C_Mem_Read(&hi2c2, SM5852_ADDR << 1, CORRECTED_PRESSURE_LSB, 1, sm5852->buff, 1, 1000);
		HAL_I2C_Mem_Read(&hi2c2, SM5852_ADDR << 1, CORRECTED_PRESSURE_MSB, 1, sm5852->buff + 1, 1, 1000);
		HAL_I2C_Mem_Read(&hi2c2, SM5852_ADDR << 1, TEMPERATURE_LSB, 1, sm5852->buff + 2, 1, 1000);
		HAL_I2C_Mem_Read(&hi2c2, SM5852_ADDR << 1, TEMPERATURE_MSB, 1, sm5852->buff + 3, 1, 1000);
	}
}

void SM5852_Manager(SM5852_T *sm5852)
{
	float pressure, temp, psi;
	if(HAL_GetTick() - sm5852->tick > 200)
	{
		uint32_t time = HAL_GetTick();
		/* Get Data to buff */
		SM5852_GetData(sm5852);

		/* Debug read SM5852 Sensor */
		//printf("%d, %d, %d ,%d\n", sm5852->buff[0], sm5852->buff[1], sm5852->buff[2], sm5852->buff[3]);
		//printf("ValueP: %d , temp: %d \n", (sm5852->buff[1] << 6 | sm5852->buff[0]) , sm5852->buff[3] << 6 | sm5852->buff[2] );
		//printf("Pressure (psi): %.5f psi\n", (toPressure(sm5852->buff[1], sm5852->buff[0])));
		//printf("Pressure (Pa) : %.0f Pa\n", ((toPressure(sm5852->buff[1], sm5852->buff[0]) * 1034.21)/0.15));
		/* *------*		*------*	*------* */

		/* Convert Raw Value to Pressure, Temperature */
		psi = toPressure(sm5852->buff[1], sm5852->buff[0]); // psi
		//printf("psi: %.2f\n", psi);
		//psi = Update_Estimate(psi);
		//printf("psi Kalman: %.2f\n", psi);
		sm5852->pressure = (psi * 1034.21)/P_MAX; 					// Convert psi to Pa
		//printf("Pressure: %.2f\n", sm5852->pressure);
		temp = toTemp(sm5852->buff[3], sm5852->buff[2]);
		temp = Update_Estimate(temp);
		sm5852->temp = Update_Estimate(temp);
		//printf("Temp: %.2f\n", sm5852->temp);
		sm5852->tick = HAL_GetTick();
	}
}

static float toPressure(uint8_t hi, uint8_t lo)
{
  uint16_t pressureCnt = (hi << 6) | lo;
  /* Process differential pressure only positive values */
  if(pressureCnt < DigoutPmin) 
  	pressureCnt = DigoutPmin;
  else
  	pressureCnt = pressureCnt;
  /* *------*	*------* 	*------* */
  float pressure = (((float)pressureCnt - DigoutPmin)/((DigoutPmax - DigoutPmin)/0.15));
  return pressure;
}

static float toTemp(uint8_t hi, uint8_t lo)
{
  uint16_t tempCnt = hi << 6 | lo;
  float temp = (((float)tempCnt - DigoutTmin)/((DigoutTmax - DigoutTmin)/(150 + 40)) - 40);
  return temp;
}