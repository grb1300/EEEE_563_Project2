/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SigGen.h"
#include "7seg.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CLI */
osThreadId_t CLIHandle;
const osThreadAttr_t CLI_attributes = {
  .name = "CLI",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Display */
osThreadId_t DisplayHandle;
const osThreadAttr_t Display_attributes = {
  .name = "Display",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void CLI_task(void *argument);
void Display_task(void *argument); //Add the function prototype for new task

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of CLI */
  CLIHandle = osThreadNew(CLI_task, NULL, &CLI_attributes);
  /* creation of Display */
  DisplayHandle = osThreadNew(Display_task, NULL, &Display_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_CLI_task */
/**
* @brief Function implementing the CLI thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CLI_task */
__weak void CLI_task(void *argument)
{
  /* USER CODE BEGIN CLI_task */
  /* Infinite loop */
	for (;;)
	{
	    char rx;
	    static char cmd[32];        // command line buffer
	    static uint8_t idx = 0;     // current write index

	    // Receive 1 char with small timeout
	    if (HAL_UART_Receive(&huart2, (uint8_t *)&rx, 1, 10) == HAL_OK)
	    {
	        // echo back whatever we got
	        HAL_UART_Transmit(&huart2, (uint8_t *)&rx, 1, HAL_MAX_DELAY);

	        // ENTER pressed? (CR or LF)
	        if (rx == '\r' || rx == '\n')
	        {
	            	uart_print("\r\n");

	            if (idx > 0)
	            {
	                cmd[idx] = '\0';  // null-terminate the command string
//	                for (int i = 0; cmd[i] != '\0'; i++) {
//	                	if (cmd[i] == '\b'){
//	                		//now we want a string that is cmd but with the backspace character and character before backspace removed (only if applicable)
//	                	}
//	                }
	                uint8_t parsed_command_value = SigGen_ParseCommand(cmd);
	                if (parsed_command_value)
	                {
	                    uart_print("OK\r\n");
	                }
	                else
	                {
	                    uart_print("ERR: use \"sine|square <100-9999>\"\r\n");
	                }
	            }

	            // reset buffer for next command
	            idx = 0;
	        }
	        else
	        {
	            // normal character → add to command buffer if space
	            if (idx < sizeof(cmd) - 1)
	            {
	                cmd[idx++] = rx;
	            }
	            else
	            {
	                // buffer full: warn and reset
	                uart_print("\r\nERR: command too long\r\n");
	                idx = 0;
	            }
	        }
	    }

	    vTaskDelay(1);   // let other tasks run
	}
  }
  /* USER CODE END CLI_task */

/* USER CODE BEGIN Display_task */
void Display_task(void *argument)
{
	  extern volatile float    Current_Frequency;
	  extern volatile uint32_t Last_Edge_Time_ms;
	  extern volatile uint32_t g_ic_edge_count;
	  uint32_t last_print_disp = 0;   // last value we printed to UART
	  uint32_t last_edge_count  = 0;
	  multiplexSegment(1234); //default state
	  for (;;){
		    uint32_t now_ms   = HAL_GetTick();
		    uint32_t last_ms  = Last_Edge_Time_ms;
		    uint32_t disp = 1234;
		    int  signal_present = 0;
		    if ((now_ms - last_ms) <= 1000u){
		    	//1 second has passed
		    	signal_present = 1;
		    	float f = Current_Frequency;
		    	if (f){
		    		disp = (uint32_t)(f + 0.5f);  // round to nearest Hz
			        if (disp > 9999u) disp = 9999u;
			        if (disp < 100u)  disp = 100u;
		    	} else {
		    		disp = 1234;
		    	}
		    } else {
			    disp = 1234;
		    }
			multiplexSegment(disp);
//			uint32_t ec = g_ic_edge_count;
//	        if (ec != last_edge_count) {
//	            char buf[96];
//	            int len = snprintf(buf, sizeof(buf),
//	                "\r\n[DBG] sig=%d now=%lu last=%lu dt=%lu edges=%lu\r\n",
//	                signal_present,
//	                (unsigned long)now_ms,
//	                (unsigned long)last_ms,
//	                (unsigned long)(now_ms - last_ms),
//	                (unsigned long)ec);
//	            HAL_UART_Transmit(&huart2, (uint8_t *)buf, len, HAL_MAX_DELAY);
//	            last_edge_count = ec;
//	        }
			if (signal_present){
				if (disp != last_print_disp){
			        char buf[64];
			        int len = snprintf(buf, sizeof(buf),"\r\nMeasured frequency: %lu Hz\r\n", (unsigned long)disp);
			        HAL_UART_Transmit(&huart2, (uint8_t *)buf, len, HAL_MAX_DELAY);
			        last_print_disp = disp;
				}
			}
			vTaskDelay(50);
	  }
}
/* USER CODE END Display_task */


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

