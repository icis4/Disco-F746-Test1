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

#include "fatfs.h"

#include "app/application.h"
#include "app/mcuutils.h"

#include "app/mcuutils.h"
#include "app/tools.h"

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
  extern uint8_t resultQSPI;
  printf("\n*** START ***\n");
  printf("\nID:%08lx%08lx%08lx\n", HAL_GetUIDw0(), HAL_GetUIDw1(), HAL_GetUIDw2());

  InitApplication();

  /* Flash Disk */
  if (resultQSPI == HAL_OK) {
	  printf("[PASS]");
	  MX_FATFS_Init();
  } else {
	  printf("[%s]", strresult(resultQSPI));
  }
  puts(" QSPI");

  if (retSDRAMDISK == FR_OK)
	  printf("[PASS] RAM Disk %s\n", SDRAMDISKPath);
  else
	  printf("[FAIL:%d] RAM Disk\n", retSDRAMDISK);

  if (retSD == FR_OK)
	  printf("[PASS] SD Disk %s\n", SDPath);
  else
	  printf("[FAIL:%d] SD Disk\n", retSD);

  if (retUSER == FR_OK)
	  printf("[PASS] USER Disk %s\n", USERPath);
  else
	  printf("[FAIL:%d] USER Disk\n", retUSER);

  uint32_t time = osKernelGetTickCount();
  BSP_DelayMicros(20000);
  printf("20000us = %ldms", osKernelGetTickCount() - time);


  printf("\n*** Ready ***\n");


  // vTaskDelete(0);

  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
}

