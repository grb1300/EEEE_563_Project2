/*
 * clock.c
 *
 *  Created on: Jul 16, 2021
 *      Author: rickweil
 *
 *  Updated on: Sept 25, 2025
 *      Author: Parikh
 *      Added support for PLL 80MHz clock as well as HSI 16MHz clock
 */


#include "stm32l476xx.h"

void clock_init(void)
{
	// 1. Enable HSI (16 MHz internal oscillator)
	RCC->CR |= ((uint32_t)RCC_CR_HSION);

    // wait until HSI is ready
    while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}

    // 2. Configure Flash latency for 80 MHz
    FLASH->ACR |= FLASH_ACR_LATENCY_4WS;
    while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_4WS);

    // MP-Sept-25:
    //    // 3. ***** USE this code for PLL clock (80MHz) *****
    //
    // Disable PLL before config
    RCC->CR &= ~RCC_CR_PLLON;
    while (RCC->CR & RCC_CR_PLLRDY);  // wait until PLL fully stops

    // Configure PLL
    RCC->PLLCFGR = 0;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;    // source HSI16
    RCC->PLLCFGR |= (1 << RCC_PLLCFGR_PLLM_Pos); // PLLM = 2 (divide by 2)
    RCC->PLLCFGR |= (20 << RCC_PLLCFGR_PLLN_Pos); // PLLN = 20 (multiply by 20)
    RCC->PLLCFGR |= (0 << RCC_PLLCFGR_PLLR_Pos);  // PLLR = 2 (divide by 2)
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;           // Enable PLLR output

    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);  // wait until PLL ready

    // Select PLL as system clock source
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;    // select PLL as system clock
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // wait for switch

    // MP-Sept-25:
    //    // 3. ***** USE this code for PLL clock (80MHz) *****


//    // 3. ***** USE this code for HSI clock (16MHz) *****
//
//    // Select HSI as system clock source
//    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
//    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;     // 01: HSI16 oscillator used as system clock
//
//    // Wait till HSI is used as system clock source
//    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 ) {;}
//
//    // 3. ***** USE this code for HSI clock (16MHz) *****


    // 4. Update System Clock to ensure its set correctly
    SystemCoreClockUpdate();
}
