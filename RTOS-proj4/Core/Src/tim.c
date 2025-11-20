/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
#include "tim.h"
#include "7seg.h"
#include "main.h"
/* USER CODE BEGIN 0 */
#define SIG_WAVE_SAMPLES   100u            	// N samples per period
#define SIG_TIMER_CLK_HZ   80000000u       	// Could be same as System Clock, adjust if your TIM6 clock differs
#define SIG_FREQ_MIN_HZ    100u				// Freq range 100 - 9999 Hz
#define SIG_FREQ_MAX_HZ    9999u			// Freq range 100 - 9999 Hz
#define SIG_PSC            0u              	// prescaler (keep 0 for best resolution)
#define NUM_PERIOD_SAMPLES 100u				// We want 100 samples for our period
volatile int First_Rising_Edge = 0; 					// do we already have a last capture?
volatile uint32_t Edge_Value1 = 0;					// The time step we see our rising edge
volatile uint32_t Edge_Value2 = 0;					// The time step we see our falling edge
volatile uint32_t Edge_value_difference = 0;			// The difference between rising and falling edge.
volatile float Current_Frequency = 0;				// The frequency calculated using the prior steps Edge_value_difference
volatile float Total_Frequency = 0;
volatile int Current_Sample = 0;
volatile uint64_t sum_period_ticks = 0;
volatile uint32_t Last_Edge_Time_ms   = 0;
volatile uint32_t g_ic_edge_count = 0;

/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim15;

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */
  //uart_print("we init timer2"); //we successfully see this
  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  //TIM_SlaveConfigTypeDef sSlaveConfig = {0}; //turned this off to make clock independent
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 79;   // 80MHz / 80 = 1 MHz
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFFFFFF;
  //Measured Frequency = (TIM2 Frequency / (PSC + 1)) / ticks per period
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; //Disabled preload
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
//  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE; // Grant disabled TIM_SLAVEMODE_TRIGGER;
//  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
//  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  //This is the code that enables TIM2 to be in input mode, we also had to disable the slavemode trigger
  //Now we wire this in with main using HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1) and we can read in values by creating the HAL_TIM_IC_CaptureCallback function
  TIM_IC_InitTypeDef sConfigIC = {0};
  HAL_TIM_IC_Init(&htim2);
  sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter    = 0;
  HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1);
  /* USER CODE END TIM2_Init 2 */

}
/* TIM6 init function */
void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = SIG_PSC;//Grant Debug used to be 79;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 7999u; //Grant Debug used to be 49;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}
/* TIM15 init function */
void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 79;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 249;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */

}

// ------------------------------ HAL_TIM_IC_CaptureCallback() ----------------------------------------------
//Auto Generated summary of what a CaptureCallback function does:
//This function is executed when a timer's input capture module detects an edge on a configured input pin.
//Its purpose is to process the captured timestamp, which is used to calculate the time between signal edges
//to determine the input signal's frequency, period, and/or pulse width. The user writes code inside this
//callback to read the captured values, typically from the Capture/Compare (CCR) registers, and perform the
//necessary calculations.
//-----------------------------------------------------------------------------------------------------------
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	//Followed the tutorial on the controllerstech.com website: How to use Timer Input Capture Mode in STM32
	//htim->Instance and htim->Channel point the CaptureCallback to the correct timer and channel, but this is unnecessary as we have only 1 read in timer
	//We would point at HAL_TIM_ACTIVATE_CHANNEL_1 and TIM2 otherwise
	//uart_print("we have entered HAL_TIM_IC_CaptureCallback");
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); //"we have entered HAL_TIM_IC_CaptureCallback"
	if (htim->Instance != TIM2 || htim->Channel != HAL_TIM_ACTIVE_CHANNEL_1){
		return;
	}
	g_ic_edge_count++;
	Last_Edge_Time_ms = HAL_GetTick();

	if (!First_Rising_Edge){
		Edge_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		First_Rising_Edge = 1;
		//Current_Sample ++; Debugging error. Here Sample represents our period. By rising it on every edge we were getting a frequency that was double our intended
		return;
	} else {
		Edge_Value2 = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
		if (Edge_Value2 > Edge_Value1){
			Edge_value_difference = Edge_Value2 - Edge_Value1;
		} else if (Edge_Value1 > Edge_Value2){ //Does this constitutes an edge case where our timer reset so thats why we see Edge_Value1 larger?
			Edge_value_difference = (0xFFFFFFFFu - Edge_Value1) + Edge_Value2 + 1u; //does this need to be cast as a signed value? We see 0xFFFFFFFF becuase this is TIM2 Period
		} else if (Edge_Value1 == Edge_Value2){
			return; //Should not see this error condition
		}

		Current_Sample ++;
		sum_period_ticks += Edge_value_difference;

		if (Current_Sample >= NUM_PERIOD_SAMPLES){
			uint32_t avg_ticks = (uint32_t)(sum_period_ticks / NUM_PERIOD_SAMPLES);
		    if (avg_ticks){
		    	float refClock = (float)HAL_RCC_GetPCLK1Freq() / (float)(htim2.Init.Prescaler + 1); //is TIMCLOCK the same as SIG_TIMER_CLK_HZ?
		        // F = timer_clk / ticks_per_period
		        Current_Frequency = refClock / (float)avg_ticks; //we should repeat this 100 times to match our problem formulation?
		        uint32_t disp = (uint32_t)(Current_Frequency + 0.5f); //we round
		        if (disp > 9999u) disp = 9999u;
		        if (disp < 100u)  disp = 100u;
		        //multiplexSegment(disp);
		    }
		    // Reset accumulation for next 100 periods
		    sum_period_ticks  = 0;
		    Current_Sample    = 0;
		}
		//__HAL_TIM_SET_COUNTER(&htim2, 0); //reset our counter
		First_Rising_Edge = 0; //we have viewed on period now?
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  /* USER CODE BEGIN TIM2_MspInit 1 */
    HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0); //Must enable TIM2 NVIC
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspInit 0 */

  /* USER CODE END TIM6_MspInit 0 */
    /* TIM6 clock enable */
    __HAL_RCC_TIM6_CLK_ENABLE();
  /* USER CODE BEGIN TIM6_MspInit 1 */

  /* USER CODE END TIM6_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM15)
  {
  /* USER CODE BEGIN TIM15_MspInit 0 */

  /* USER CODE END TIM15_MspInit 0 */
    /* TIM15 clock enable */
    __HAL_RCC_TIM15_CLK_ENABLE();

    /* TIM15 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
  /* USER CODE BEGIN TIM15_MspInit 1 */

  /* USER CODE END TIM15_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspDeInit 0 */

  /* USER CODE END TIM6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM6_CLK_DISABLE();
  /* USER CODE BEGIN TIM6_MspDeInit 1 */

  /* USER CODE END TIM6_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM15)
  {
  /* USER CODE BEGIN TIM15_MspDeInit 0 */

  /* USER CODE END TIM15_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM15_CLK_DISABLE();

    /* TIM15 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_BRK_TIM15_IRQn);
  /* USER CODE BEGIN TIM15_MspDeInit 1 */

  /* USER CODE END TIM15_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM15){
		// Update the display
		displaySegment();
	}

}

/* USER CODE END 1 */
