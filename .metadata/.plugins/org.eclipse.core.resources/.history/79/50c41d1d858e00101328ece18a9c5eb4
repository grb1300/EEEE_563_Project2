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
#include "demo.h"			// Demo Module --- This shoudl be replaced with your LED Control ---
#include "clock.h"			// Clock
#include "gpio.h"			// GPIO Module

#define LED_PIN 5

// Default Entry Point
int main(void)
{
	// initialization Clock, LED, SysTick, USART and GPIO Modules
	clock_init();
	LED_Init();
	init_systick() ;
	USART2_Init(115200);
	GPIO_Init();

	// Run Demo
	printf("%s\r\n", "*** TEST ***");
	run_demo();
}

