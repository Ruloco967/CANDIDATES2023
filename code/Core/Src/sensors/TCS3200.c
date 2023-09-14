#include "sensors/TCS3200.h"

#include "usart.h"

#include <stdio.h>

// Codigo basado en https://github.com/jaimelaborda/TCS3200_STM32F4_Library

volatile uint8_t state = IDLE;
volatile uint8_t message[35] = {'\0'};
volatile uint32_t T1 = 0;
volatile uint32_t T2 = 0;
volatile uint32_t ticks = 0;
volatile uint16_t TIM2_OVC = 0;
volatile uint32_t frequency = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
{
    if(state == IDLE)
    {
        T1 = TIM2->CCR1;
        TIM2_OVC = 0;
        state = DONE;
    }
    else if(state == DONE)
    {
        T2 = TIM2->CCR1;
        ticks = (T2 + (TIM2_OVC * 65536)) - T1;
        frequency = (uint32_t)(F_CLK/ticks);
        if(frequency != 0)
        {
	    sprintf(message, "Frequency = %lu Hz\n\r", frequency);
	    HAL_UART_Transmit(&huart1, message, sizeof(message), 100);
        }
        state = IDLE;
    }
}

void TCS3200_config(void)
{

}
