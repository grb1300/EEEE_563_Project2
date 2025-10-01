/*
 * timer.c
 *
 *  Created on: Sep 21, 2025
 *      Author: Mitesh Parikh
 */

/* Includes ------------------------------------------------------------------*/
#include "timer.h"
#include "uart.h"
#include "stm32l476xx.h"
#include <stdbool.h>

static volatile uint32_t last_capture = 0;
static volatile uint32_t period_ticks = 0;
static volatile bool first_capture = true;

//******************************************************************************************
// This function is to Initialize Timer 2
//******************************************************************************************
void TIM2_Init(void)
{
	// reset TIM2 before configuration
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_TIM2RST;
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_TIM2RST;

	// 1) Enable TIM2 clock (APB1)
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	// 2) Prescale TIM2 to 1 MHz (1 µs per tick)
	//    Counter clk = SystemCoreClock / (PSC+1)
	// MP-Sept-25: Fixed issue
	// When we write value to PSC register, it stored info in preload register (like shadow register).
	// New setting will take into effect only after an Update event. Since we were setting ARR as 0xFFFFFFFF,
	// we have to wait unit counter resets to see new PSC value applied. This is the reason why i was seeing
	// behavior where my Period/Frequency values were incorrect for 3-5 minutes. Once counter resets and
	// new PSC value takes into effect, i started to see correct period and freq values.
	uint32_t psc = (SystemCoreClock / 1000000UL) - 1UL;
	TIM2->PSC = psc;           // e.g., at 16 MHz, PSC = 15
	TIM2->EGR |= TIM_EGR_UG;  // generate an update event to reload PSC immediately
	TIM2->ARR = 0xFFFFFFFFUL;  // free-run 32-bit (TIM2 is 32-bit on L4)

	printf("SystemCoreClock = %lu, TIM2 PSC = %lu\r\n", SystemCoreClock, TIM2->PSC);

	// 3) Configure CH1 as input, mapped to TI1
	//    CC1S = 01 (input on TI1)
	TIM2->CCMR1 &= ~(TIM_CCMR1_CC1S_Msk);
	TIM2->CCMR1 |=  (1u << TIM_CCMR1_CC1S_Pos);   	// CC1S=01 = TI1 input

	// Optional digital filter (IC1F). 0 = no filter; choose >0 if you have a noisy signal
	TIM2->CCMR1 &= ~(TIM_CCMR1_IC1F_Msk);

	// Select rising edge (CC1P=0, CC1NP=0)
	TIM2->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);

	// No prescaler on capture (IC1PSC=00) => capture every edge (i.e. rising edge)
	TIM2->CCMR1 &= ~(TIM_CCMR1_IC1PSC_Msk);

	// 4) Enable interrupt on capture
	TIM2->DIER |= TIM_DIER_CC1IE;

	// 5) Enable capture and counter
	TIM2->CCER |= TIM_CCER_CC1E;
	TIM2->CR1  |= TIM_CR1_CEN;

	// 6) Clear all interrupt flags before starting
	TIM2->SR = 0;

	// 7) Enable NVIC Interrupt
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 1);
	NVIC_EnableIRQ(TIM2_IRQn);
}

//******************************************************************************************
// This function is to handle Timer2 Interrupt, triggered when rising edge is detected
//******************************************************************************************
void TIM2_IRQHandler(void)
{
	// Capture event on CH1 only
	if (TIM2->SR & TIM_SR_CC1IF)
    {
		// Current captured counter
		uint32_t current_ticks = TIM2->CCR1;

        // MP-Sept-25: Revised and simplified code
        // Let's skip first reading to ensure we have valid signal to work with
        if (!first_capture)
        {
        	// Calculate period using current and last rising edge values
        	// This should handle overflow automatically
        	period_ticks = (current_ticks - last_capture);
        }
        else
        {
        	// Skip very first reading
        	first_capture = false;
        }
        // Save for next edge
        last_capture = current_ticks;

    	// Clear flag
    	TIM2->SR &= ~TIM_SR_CC1IF;
    }
}





//******************************************************************************************
// This function is to get Period of input signal measured
//******************************************************************************************
uint32_t TIM2_GetPeriodTicks(void)
{
	return period_ticks;	// already in µs since PSC is set accordingly
}
