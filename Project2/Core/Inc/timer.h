/*
 * timer.h
 *
 *  Created on: Sep 21, 2025
 *      Author: Mitesh Parikh
 */

#ifndef TIMER_H_
#define TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

// Timer Initialization
void TIM2_Init(void);
uint32_t TIM2_GetPeriodTicks(void);

#endif /* TIMER_H_ */
