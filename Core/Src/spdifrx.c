/**
  ******************************************************************************
  * File Name          : SPDIFRX.c
  * Description        : This file provides code for the configuration
  *                      of the SPDIFRX instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "spdifrx.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPDIFRX_HandleTypeDef hspdif;

/* SPDIFRX init function */
void MX_SPDIFRX_Init(void)
{

  hspdif.Instance = SPDIFRX;
  hspdif.Init.InputSelection = SPDIFRX_INPUT_IN0;
  hspdif.Init.Retries = SPDIFRX_MAXRETRIES_NONE;
  hspdif.Init.WaitForActivity = SPDIFRX_WAITFORACTIVITY_OFF;
  hspdif.Init.ChannelSelection = SPDIFRX_CHANNEL_A;
  hspdif.Init.DataFormat = SPDIFRX_DATAFORMAT_LSB;
  hspdif.Init.StereoMode = SPDIFRX_STEREOMODE_DISABLE;
  hspdif.Init.PreambleTypeMask = SPDIFRX_PREAMBLETYPEMASK_OFF;
  hspdif.Init.ChannelStatusMask = SPDIFRX_CHANNELSTATUS_OFF;
  hspdif.Init.ValidityBitMask = SPDIFRX_VALIDITYMASK_OFF;
  hspdif.Init.ParityErrorMask = SPDIFRX_PARITYERRORMASK_OFF;
  if (HAL_SPDIFRX_Init(&hspdif) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SPDIFRX_MspInit(SPDIFRX_HandleTypeDef* spdifrxHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spdifrxHandle->Instance==SPDIFRX)
  {
  /* USER CODE BEGIN SPDIFRX_MspInit 0 */

  /* USER CODE END SPDIFRX_MspInit 0 */
    /* SPDIFRX clock enable */
    __HAL_RCC_SPDIFRX_CLK_ENABLE();
  
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SPDIFRX GPIO Configuration    
    PD7     ------> SPDIFRX_IN0 
    */
    GPIO_InitStruct.Pin = SPDIF_RX0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_SPDIFRX;
    HAL_GPIO_Init(SPDIF_RX0_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN SPDIFRX_MspInit 1 */

  /* USER CODE END SPDIFRX_MspInit 1 */
  }
}

void HAL_SPDIFRX_MspDeInit(SPDIFRX_HandleTypeDef* spdifrxHandle)
{

  if(spdifrxHandle->Instance==SPDIFRX)
  {
  /* USER CODE BEGIN SPDIFRX_MspDeInit 0 */

  /* USER CODE END SPDIFRX_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPDIFRX_CLK_DISABLE();
  
    /**SPDIFRX GPIO Configuration    
    PD7     ------> SPDIFRX_IN0 
    */
    HAL_GPIO_DeInit(SPDIF_RX0_GPIO_Port, SPDIF_RX0_Pin);

  /* USER CODE BEGIN SPDIFRX_MspDeInit 1 */

  /* USER CODE END SPDIFRX_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
