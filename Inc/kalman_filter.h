#ifndef __KALMAN_FILTER_H_
#define __KALMAN_FILTER_H_

#include "main.h"
#include <math.h>

typedef struct
{
	float _err_measure;
	float _err_estimate;
	float _q;
	float _current_estimate;
	float _last_estimate;
	float _kalman_gain;
} KALMAN_FILTER_T;

extern KALMAN_FILTER_T kalman;

void Simple_Kalman_Filter(float mea_e, float est_e, float q);
float Update_Estimate(float mea);
void Set_Measurement_Error(float mea_e);
void Set_Estimate_Error(float est_e);
void Set_Process_Noise(float q);
float Get_Kalman_Gain(void);
float Get_EstimateError(void);
#endif