/*
 * delay_ms.c
 *
 *  Created on: 20.04.2020 г.
 *      Author: icis4
 */
#include <stdio.h>
#include "tim.h"


TIM_HandleTypeDef htimDelay;

void configureTimerForDelay_us(void)
{
	  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	  TIM_MasterConfigTypeDef sMasterConfig = {0};

	  RCC_ClkInitTypeDef    clkconfig;
	  uint32_t              uwTimclock = 0;
	  uint32_t              uwPrescalerValue = 0;
	  uint32_t              pFLatency;

	  /* Enable TIMx clock */
	  __HAL_RCC_TIM2_CLK_ENABLE();

	  /* Get clock configuration */
	  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

	  /* Compute TIMx clock */
	  uwTimclock = 2 * HAL_RCC_GetPCLK1Freq();

	  /* Compute the prescaler value to have TIMx counter clock equal to 1MHz */
	  uwPrescalerValue = (uint32_t) ((uwTimclock / 10000000) - 1);

	  /* Initialize TIMx */
	  htimDelay.Instance = TIM2;

	  /* Initialize TIMx peripheral as follow:
	  + Period = [(TIMxCLK/1000) - 1]. to have a (1/1000) s time base.
	  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
	  + ClockDivision = 0
	  + Counter direction = Up
	  */
	  htimDelay.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htimDelay.Init.Period = 0xFFFFFFFF;
	  htimDelay.Init.Prescaler = uwPrescalerValue;
	  htimDelay.Init.ClockDivision = 0;
	  if(HAL_TIM_Base_Init(&htimDelay) != HAL_OK)
	  {
		  printf("FreeRTOS Delay_ms Timer Not Started\n");
		  return;
	  }

	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&htimDelay, &sClockSourceConfig) != HAL_OK)
	  {
		  printf("FreeRTOS Delay_ms Timer Not Started\n");
		  return;
	  }

	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htimDelay, &sMasterConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_TIM_Base_Start(&htimDelay);
	  /* Return function status */
	  printf("[PASS] Delay_us Timer Started\n");
	  return;
}

/* Dalay in microseconds using Timer2 at APB1 clock */
void BSP_DelayMicros(uint32_t microseconds)
{
	uint32_t count_timer, end_period;

	count_timer = htimDelay.Instance->CNT;
	/* Wait front */
	while(count_timer == htimDelay.Instance->CNT)
		;
	count_timer = htimDelay.Instance->CNT;

	/* Minimal interval up to 2 clocks */
	if (microseconds == 0) {
		count_timer = htimDelay.Instance->CNT;
		/* Wait front */
		while(count_timer == htimDelay.Instance->CNT)
			;
		return;
	}

	end_period = count_timer + microseconds * 10; // APB1 Timer clock Freq in MHz, prescaler 0

	while (end_period <= count_timer && htimDelay.Instance->CNT > count_timer)
		;
	while (htimDelay.Instance->CNT <= end_period)
		;
}

uint32_t BSP_GetMicros(void)
{
	return htimDelay.Instance->CNT / 10;
}


