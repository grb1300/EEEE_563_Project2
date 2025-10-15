/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  *
  * Prepared by Mitesh Parikh - Sept 2025
  *
  ******************************************************************************
  */

/* --------------------
 * Includes
 * --------------------*/
#include "uart.h"			//UART Module
#include "LED.h"			// LED Control
#include "demo.h"			// Demo Module --- This should be replaced with your LED Control ---
#include "clock.h"			// Clock
#include "gpio.h"			// GPIO Module
#include "timer.h"			// Timer module

	
// Default Entry Point
int main(void)
{
	// initialization Clock, LED, SysTick, USART and GPIO Modules
	clock_init();
	GPIO_Init();
	init_systick();
	USART2_Init(115200);
	USART2_WriteString("Complete Clock, GPIO and UART config!\r\n");
	LED_Init();
	// Timer Initialization for Project 2
    TIM2_Init();

	// Run Demo
	run_demo();
}

