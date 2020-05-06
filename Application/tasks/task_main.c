/*
 * task_main.c
 *
 *  Created on: Feb 14, 2020
 *      Author: icis4
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "fatfs.h"

#include "app/application.h"
#include "app/mcuutils.h"

#include "app/mcuutils.h"
#include "app/tools.h"

extern uint8_t resultQSPI;

extern void MX_LWIP_Init(void);
extern int diskfree(char* path, DWORD *total_kb, DWORD *free_kb);

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

  printf("\n*** START ***\n");
  printf("\nID:%08lx%08lx%08lx\n", HAL_GetUIDw0(), HAL_GetUIDw1(), HAL_GetUIDw2());

  MX_LWIP_Init();

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

//  uint32_t time = osKernelGetTickCount();
//  BSP_DelayMicros(20000);
//  printf("20000us = %ldms", osKernelGetTickCount() - time);

  printf("\n*** Ready ***\n");

#if 0
  //FINE!
  extern int duktape_main();
  duktape_main();
#endif

#if 0
  //FINE!
  void test_fractal();
  test_fractal();
#endif

	DWORD total_kb, free_kb;
	int result;

	printf("\n- DiskFree -\n");

	/* FATfs RAM Disk*/
	result = diskfree(SDRAMDISKPath, &total_kb, &free_kb);
	if (!result) printf("%s - %lu KiB total drive space. %lu KiB available.\n", SDRAMDISKPath, total_kb, free_kb);
	else printf("%s - Error!\n", SDRAMDISKPath);

	/* FATfs SD Card */
	result = diskfree(SDPath, &total_kb, &free_kb);
	if (!result) printf("%s - %lu MiB total drive space. %lu MiB available.\n", SDPath, total_kb / 1024, free_kb / 1024);
	else printf("%s - Error!\n", SDPath);

	/* FATfs Flash Disk*/
	result = diskfree(USERPath, &total_kb, &free_kb);
	if (!result) printf("%s - %lu KiB total drive space. %lu KiB available.\n", USERPath, total_kb, free_kb);
	else printf("%s - Error:%d\n", USERPath, errno);

  // vTaskDelete(0);

  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
}

