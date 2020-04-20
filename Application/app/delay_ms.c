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
	  htimDelay.Init.Period = (1000000 / 100000) - 1; // 1MHz
	  htimDelay.Init.Prescaler = uwPrescalerValue;
	  htimDelay.Init.ClockDivision = 0;
	  htimDelay.Init.CounterMode = TIM_COUNTERMODE_UP;
	  if(HAL_TIM_Base_Init(&htimDelay) != HAL_OK)
	  {
		  printf("FreeRTOS Delay_ms Timer Not Started\n");
		  return;
	  }

	  /* Return function status */
	  printf("FreeRTOS Delay_ms Timer Started\n");
	  return;
}

/* Dalay in microseconds using Timer2 at APB1 clock */
void BSP_DelayMicros(uint32_t microseconds)
{
	uint32_t count_timer, end_period;

	count_timer = htim2.Instance->CNT;
	/* Wait front */
	while(count_timer == htim2.Instance->CNT)
		;
	count_timer = htim2.Instance->CNT;

	/* Minimal interval up to 2 clocks */
	if (microseconds == 0) {
		count_timer = htim2.Instance->CNT;
		/* Wait front */
		while(count_timer == htim2.Instance->CNT)
			;
		return;
	}

	end_period = count_timer + microseconds * 10; // APB1 Timer clock Freq in MHz, prescaler 0

	while (end_period <= count_timer && htim2.Instance->CNT > count_timer)
		;
	while (htim2.Instance->CNT <= end_period)
		;
}

uint32_t BSP_GetMicros(void)
{
	return htim2.Instance->CNT / 10;
}


