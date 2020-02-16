/*
 * task_main.c
 *
 *  Created on: Feb 14, 2020
 *      Author: icis4
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app/application.h"
#include "app/mcuutils.h"


/**
  * @brief  Default task
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
__NO_RETURN void StartDefaultTask(void *argument)
{
  InitApplication();

  printf("\nID:%08lx%08lx%08lx\n", HAL_GetUIDw0(), HAL_GetUIDw1(), HAL_GetUIDw2());
  printf("*** Ready ***\n");

  // vTaskDelete(0);

  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
}

