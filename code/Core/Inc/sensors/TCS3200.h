#ifndef TCS3200_H
#define TCS3200_H

#include "stm32f1xx_hal.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include <stdint.h>

#define IDLE   0
#define DONE   1
#define F_CLK  72000000UL

#define MIN_RED 5000.0
#define MAX_RED 16400.0
#define MIN_GREEN 7000.0
#define MAX_GREEN 11000.0
#define MIN_BLUE 6000.0
#define MAX_BLUE 10000.0

enum Scaling{SCL0, SCL2, SCL20, SCL100};
enum Filter{RED, GREEN, BLUE, CLEAR};

// extern SemaphoreHandle_t xColorReadySemaphore;

extern volatile uint16_t TIM2_OVC;

extern volatile _Bool IC_ColorMode;
extern volatile uint8_t calibrate_num;
extern volatile uint16_t TimeColor_High, TimeColor_Low, TimeColor;

void TCS3200_Scaling(uint8_t mode);
void TCS3200_Filter(uint8_t color);
uint8_t TCS3200_GetColor(uint8_t color);
#endif
