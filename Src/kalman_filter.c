#include "kalman_filter.h"

KALMAN_FILTER_T kalman;

void Simple_Kalman_Filter(float mea_e, float est_e, float q)
{
	kalman._err_measure = mea_e;
	kalman._err_estimate = est_e;
	kalman._q = q;
}

float Update_Estimate(float mea)
{
	kalman._kalman_gain = kalman._err_estimate/(kalman._err_estimate + kalman._err_measure);
	kalman._current_estimate = kalman._last_estimate + kalman._kalman_gain * (mea - kalman._last_estimate);
	kalman._err_estimate = (1.0 - kalman._kalman_gain)*kalman._err_estimate + fabs(kalman._last_estimate - kalman._current_estimate)*kalman._q;
	kalman._last_estimate = kalman._current_estimate;
	return kalman._current_estimate;
}

void Set_Measurement_Error(float mea_e)
{
	kalman._err_measure = mea_e;
}

void Set_Estimate_Error(float est_e)
{
	kalman._err_estimate = est_e;
}

void Set_Process_Noise(float q)
{
	kalman._q = q;
}

float Get_Kalman_Gain(void)
{
	return kalman._kalman_gain;
}

float Get_EstimateError(void)
{
	return kalman._err_estimate;
}

