/*
 * 7seg.c
 *
 *      Author: Mitesh Parikh
 */

#ifndef INC_SEVEN_SEG_H_
#define INC_SEVEN_SEG_H_

#include "stm32l4xx_hal.h"

#define LOW 		GPIO_PIN_RESET
#define HIGH 		GPIO_PIN_SET

#define dataS(x)	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, x? HIGH:LOW)
#define ShiftCLK(x) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, x? HIGH:LOW)
#define LatchCLK(x) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, x? HIGH:LOW)

void initSeg();
void displaySegment(void);
void multiplexSegment(uint16_t num4digit);

#endif /* INC_7_SEG_H_ */
