#ifndef TCS3200_H
#define TCS3200_H

#include "stm32f1xx_hal.h"

#define IDLE   0
#define DONE   1
#define F_CLK  72000000UL

extern volatile uint16_t TIM2_OVC;

void TCS3200_Config(void);
#endif
