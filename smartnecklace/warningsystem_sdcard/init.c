#include "declaration.h"
#include "init.h"

void vars_init()
{
	capture_index  = 0;

	ADC1_completed = false;
	ADC2_completed = false;
	ADC3_completed = false;

	first_run				   = true;
	adc_capture_completed_flag = false;
}