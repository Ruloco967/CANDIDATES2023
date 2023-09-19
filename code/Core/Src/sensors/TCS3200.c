#include "sensors/TCS3200.h"

#include "main.h"
#include "tim.h"
#include "stm32f1xx_hal_gpio.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
// #include "cmsis_os.h"

// Codigo basado en https://github.com/jaimelaborda/TCS3200_STM32F4_Library

// volatile uint8_t state = IDLE;
// volatile uint32_t T1 = 0;
// volatile uint32_t T2 = 0;
// volatile uint32_t ticks = 0;
// volatile uint16_t TIM2_OVC = 0;
// volatile uint32_t frequency = 0;
//
// volatile uint32_t red_filter[] = {0, 0, 0};
// uint8_t red_counter = 0;
//
// volatile uint32_t green_filter[] = {0, 0, 0};
// volatile uint32_t blue_filter[] = {0, 0, 0};
//
// SemaphoreHandle_t xColorReadySemaphore = NULL;

// _Bool mutex = false;


volatile uint32_t IC_Val1 = 0;
volatile uint32_t IC_Val2 = 0;
volatile uint32_t Difference = 0;
volatile uint8_t Is_First_Captured = 0;  // is the first value captured ?
volatile uint8_t Distance  = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // if the interrupt source is channel1
    {
	if (Is_First_Captured==0) // if the first value is not captured
	{
	    IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
	    Is_First_Captured = 1;  // set the first captured as true
	    // Now change the polarity to falling edge
	    __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
	}

	else if (Is_First_Captured==1)   // if the first is already captured
	{
	    IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
	    __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

	    if (IC_Val2 > IC_Val1)
	    {
		Difference = IC_Val2-IC_Val1;
	    }

	    else if (IC_Val1 > IC_Val2)
	    {
		// printf("->here\n\r");
		Difference = (0xffff - IC_Val1) + IC_Val2;
	    }

	    Distance = Difference * .034/2;
	    // printf("v1: %d\n\r", IC_Val1);
	    // printf("v2: %d\n\r", IC_Val2);
	    // printf("----------\n\r");
	    Is_First_Captured = 0; // set it back to false

	    // set polarity to rising edge
	    __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
	    __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
	}
    }
}
