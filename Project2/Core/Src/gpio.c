/*
 * gpio.c
 *
 *  Created on: Nov 4, 2021
 *      Author: Mitesh Parikh
 *
 *  Updated on: Sept 25, 2025
 *      Author: Parikh
 *      Cleaned up GPIO, removed HAL definitions
 */


/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "stm32l4xx_hal_gpio.h"

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void GPIO_Init(void)
{
    // 1. Enable GPIO clocks
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   // for LED (PA5), UART2 (PA2/PA3), TIM2_CH1 (PA0)
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;   // for User Button (PC13)

    // ----------------------------
    // Configure PA5: User LED
    // ----------------------------
    GPIOA->MODER &= ~(3u << (5*2));        // clear mode bits
    GPIOA->MODER |=  (1u << (5*2));        // 01 = output
    GPIOA->OTYPER &= ~(1u << 5);           // 0 = push-pull
    GPIOA->OSPEEDR |=  (3u << (5*2));      // very high speed
    GPIOA->PUPDR &= ~(3u << (5*2));        // no pull

    // ----------------------------
    // Configure PC13: User Button
    // ----------------------------
    GPIOC->MODER &= ~(3u << (13*2));       // 00 = input
    GPIOC->PUPDR &= ~(3u << (13*2));       // no pull (Nucleo board already has ext pull-up)

    // ----------------------------
    // Configure PA0: TIM2_CH1 input capture
    // ----------------------------
    GPIOA->MODER &= ~(3u << (0*2));
    GPIOA->MODER |=  (2u << (0*2));        					// 10 = alternate function
    GPIOA->AFR[0] &= ~(0xFu << (0*4));
    GPIOA->AFR[0] |=  (0x1u << (0*4));     					// AF1 = TIM2_CH1
    GPIOA->OSPEEDR |=  (3u << (0*2));      					// very high speed
    GPIOA->PUPDR   &= ~(3u << (0*2));
    GPIOA->PUPDR   |=  (2u << (0*2));      					// pull-down → avoids floating input when no signal
}

//******************************************************************************************
// This function is to handle GPIO Extended Interrupt
//******************************************************************************************
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

