#include "sensors/TCS3200.h"

#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "usart.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "cmsis_os.h"

// Codigo basado en https://github.com/jaimelaborda/TCS3200_STM32F4_Library

volatile uint8_t state = IDLE;
volatile uint32_t T1 = 0;
volatile uint32_t T2 = 0;
volatile uint32_t ticks = 0;
volatile uint16_t TIM2_OVC = 0;
volatile uint32_t frequency = 0;

// SemaphoreHandle_t xColorReadySemaphore = NULL;

_Bool mutex = false;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
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
            if (mutex == false) mutex = true;
            // xSemaphoreGiveFromISR(xColorReadySemaphore, NULL);
            // if ( (IC_ColorMode == true) && (calibrate_num == 0) ) {
            //     TimeColor_Low = frequency;
            //     calibrate_num = 1;
            // } else if ( (IC_ColorMode == true) && (calibrate_num == 0) ) {
            //     TimeColor_High = frequency;
            //     IC_ColorMode = false;
            //     calibrate_num = 0;
            // }
	    // printf("Frequency = %lu Hz\n\r", frequency);
        }
        state = IDLE;
    }
}

void TCS3200_Scaling(uint8_t mode) {
    switch (mode){
            case(SCL0):
                HAL_GPIO_WritePin(GPIOB, S0_Pin | S1_Pin, GPIO_PIN_RESET);
                break;
            case(SCL2):
                HAL_GPIO_WritePin(GPIOB, S0_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, S1_Pin, GPIO_PIN_SET);

                break;
            case(SCL20):
                HAL_GPIO_WritePin(GPIOB, S1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, S0_Pin, GPIO_PIN_SET);
                break;
            case(SCL100):
                HAL_GPIO_WritePin(GPIOB, S0_Pin | S1_Pin, GPIO_PIN_SET);
                break;
    }
}

void TCS3200_Filter(uint8_t color) {

    switch (color){
            case(RED):
                HAL_GPIO_WritePin(GPIOB, S2_Pin | S3_Pin, GPIO_PIN_RESET);
                break;
            case(BLUE):
                HAL_GPIO_WritePin(GPIOB, S2_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, S3_Pin, GPIO_PIN_SET);

                break;
            case(GREEN):
                HAL_GPIO_WritePin(GPIOB, S3_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, S2_Pin, GPIO_PIN_SET);
                break;
            case(CLEAR):
                HAL_GPIO_WritePin(GPIOB, S2_Pin | S3_Pin, GPIO_PIN_SET);
                break;
    }
}

uint8_t TCS3200_GetColor(enum Filter color) {
    uint8_t out_color = 0;

    TCS3200_Filter(color);
    osDelay(100);

    // if (xSemaphoreTake(xColorReadySemaphore, portMAX_DELAY) == pdTRUE) {
    if (mutex == true) {

        uint32_t ready_freq = frequency;

	switch (color){
            case RED:
                out_color = (255.0/(MAX_RED-MIN_RED))*(ready_freq-MIN_RED); //MAPEAR FUNCIÓN
                break;
            
            case GREEN:
                out_color = (255.0/(MAX_GREEN-MIN_GREEN))*(ready_freq-MIN_GREEN);  //MAPEAR FUNCIÓN
                break;
		
            case BLUE:
                out_color = (255.0/(MAX_RED-MIN_BLUE))*(ready_freq-MIN_BLUE);  //MAPEAR FUNCIÓN
                break;
	}

        TCS3200_Filter(CLEAR);

        if (out_color > 255) out_color = 255;
	if (out_color < 0) out_color = 0;
        mutex = false;
    }

    return out_color;
}
