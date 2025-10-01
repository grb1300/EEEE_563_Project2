/*
 * demo.c
 *
 *  Created on: Feb 14, 2021
 *      Author: larry kiser
 *  Updated on: Sept 9, 2025
 *      Author: Mitesh Parikh
 *
 *  Updated on: Sept 23, 2025 (added starter code for Input Capture)
 *      Author: Mitesh Parikh
 *
 *  Updated on: Sept 25, 2025 (fixed issue with PSC and updated code to show Period and Freq)
 *      Author: Mitesh Parikh
 */

// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <float.h>

// Custom Include files
#include "uart.h"
#include "stm32l476xx.h"
#include "demo.h"
#include "gpio.h"
#include "clock.h"
#include "LED.h"
// Added for Project 2
#include "timer.h"

// Global/Static variables
static uint32_t B1_Switch_Press_time_ms = 0;
static uint32_t one_second_counter = 0;
static uint8_t one_second_elapsed = 0;

// crude delay for demos
static void delay_ms(uint32_t ms) {
    // use SysTick if you already set it; this is just a busy-wait fallback
    for (volatile uint32_t i = 0; i < (ms * 8000UL); ++i) __NOP(); // ~1ms @80MHz (rough)
}

// This function is to Initialize SysTick registers
void init_systick()
{
	// Use the SysTick global structure pointer to do the following in this
	// exact order with separate lines for each step:
	//
	// Disable SysTick by clearing the CTRL (CSR) register.
	SysTick->CTRL = 0UL;

	// Set the LOAD (RVR) to 15,999 to give us a 1 millisecond timer
	// System clock is 16MHz
	SysTick->LOAD |= (SysTick_LOAD_RELOAD_Msk & 15999UL);

	// Set the clock source bit in the CTRL (CSR) to the internal clock.
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

	// Enable the interrupt bit
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

	// Set the enable bit in the CTRL (CSR) to start the timer.
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
}

//******************************************************************************************
// This function is to handle SysTick Timer
//******************************************************************************************
void SysTick_Handler(void)
{
	//In this example, we will have LED blinking at One second interval so first Update One Second Counter
	//three_second_counter++;
	one_second_counter++;

	// Set a global One second elapsed flag when the handler has been called 1000 times
	// Once flag is set, reset the counter
	if (one_second_counter == 1000)
	{
		one_second_elapsed = true;
		one_second_counter = 0;
	}
}




//******************************************************************************************
// This function is to handle interrupts generated because of pressing B1 switch
//******************************************************************************************
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_13)
	{
		// Now that User pressed B1 switch, check how long switch is in pressed state
		// if Switch is released within 3 seconds then toggle between Auto and Manual modes
		// Add your code ---

		// If more than 3 second then Switch to Pattern Mode

		// Reset Switch time (in milliseconds)
		B1_Switch_Press_time_ms = 0;

		while(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
		{
			// Blocking code --

			// We can use SysTick Interrupt to keep track of time
			while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
			{
				// do nothing -- Blocking code -- 1ms timer
			}
			B1_Switch_Press_time_ms++;

			// If we are in Auto mode and B1 Switch is Pressed for 3 Seconds or more then Switch to Pattern Mode
			// ---- Add your code
		}
	}
}

//******************************************************************************************
// Run Demo -- main function with forever loop
//******************************************************************************************
void run_demo( void )
{
	// Default mode is Auto Mode
	printf("%s\r\n", "*** AUTO MODE ***");

	// Set buffer for Command Buffer
	// uint32_t index = 0;
	//memset(command_buffer, 0, 100);

	// Start forever loop
	while(1)
	{
		// Read USART to see if USER typed any commands
		//char one_char = USART_Read_NB(USART2);
		//(void) one_char;	/// avoid compiler warning

		// Switch between Modes

		// If Current mode is Auto Mode then.. Run Auto Mode
			// While in Auto Mode..
			//		look for One second timer flag and toggle LED if flag is set
			//		Reset flag
			//
		// If Current mode is Manual Mode then.. Run Manual Mode
			// While in Manual Mode ..
			//		If received character from USART is not an Enter Key
			//			then keep adding character in the command array
			//		If received character from USART is an Enter key
			//			Check whether received command is "ON" -> Turn on LED
			//			Check whether received command is "OFF" -> Turn off LED
			//			If Command is neither "ON" or "OFF" then it is Invalid command
			//	In all cases update user by showing received command and whether command is valid or not
		// If Current mode is Pattern Mode then.. Run Pattern Mode
			// While in Pattern Mode:
			//		Use one second timer flag and current Pattern to toggle LED accordingly
			//		If received character is an ENTER key then
			//			check whether received command is "EXIT" -> Switch to Auto Mode
			//			check whether received command start with "P" -> update current pattern with new Pattern
			//				Please ensure pattern is valid (4 digits, 0's and 1's)
			//			In case of bad pattern or invalid command.. inform user accordingly

		// MP-Sept-25: Revised code for Input Capture Info, showing Period and Freq
		uint32_t ticks = TIM2_GetPeriodTicks(); // copy volatile safely
		if (ticks > 0)
		{
			float freq = 1e6f / ticks;
			printf("Period = %lu us, Freq = %.2f Hz\r\n", ticks, freq);
		}

		// MP-Sept-25: delay for demo only -- this is not required for Project as you will not be outputing anything until after
		// you have histogram is ready to display
		delay_ms(10);
	}
}
