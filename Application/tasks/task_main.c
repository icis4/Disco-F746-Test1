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

#include "lwip.h"
#include "mdns.h"

#if osCMSIS == 0x10002
#define TIME_SEC ((float)osKernelSysTick() / (float)osKernelSysTickFrequency)
#else
#define TIME_SEC ((float)osKernelGetTickCount() / (float)osKernelGetSysTimerFreq())
#endif

#define DEBUG_FS(func, ...) \
	do {\
		FRESULT res = func(__VA_ARGS__);\
		if (res != FR_OK) {\
			printf("FAT Error: %d\n", res);\
	  	}\
	} while(0)

extern struct netif gnetif;

extern uint8_t resultQSPI;

extern int diskfree(char* path, DWORD *total_kb, DWORD *free_kb);

static void srv_txt(struct mdns_service *service, void *txt_userdata)
{
 int res = mdns_resp_add_service_txtitem(service, "path=/", 6);
 LWIP_ERROR("mdns add service txt failed\n", (res == ERR_OK), return);
}

void MDNS_Init(void)
{
	/* Enable multicast for mDNS */
	(heth.Instance)->MACFFR |= ETH_MULTICASTFRAMESFILTER_NONE;
	gnetif.flags |= NETIF_FLAG_IGMP;

	mdns_resp_init();
	mdns_resp_add_netif(&gnetif, gnetif.hostname, 60);
	mdns_resp_add_service(&gnetif, "disco", "_http", DNSSD_PROTO_TCP, 80, 3600, srv_txt, NULL);
}

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

  /* Flash Disk */
  if (resultQSPI == HAL_OK) {
	  printf("[PASS]");
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

	if (netif_is_link_up(&gnetif)) {
		MDNS_Init();
		puts("[PASS] LwIP");
	} else {
		puts("[FAIL] LwIP");
	}

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
	if (result == HAL_OK)
		printf("%s - %lu KiB total drive space. %lu KiB available.\n", USERPath, total_kb, free_kb);
	else
		printf("%s - Error:%d\n", USERPath, errno);

	#if 0
	  extern int duktape_main();
	  duktape_main();
	#endif

	#if 1
	  FIL fp;
	  UINT bw;
	  FSIZE_t size;

	  DEBUG_FS(f_open, &fp, "2:/log.txt", FA_OPEN_APPEND | FA_WRITE);
	  DEBUG_FS(f_write, &fp, "1234\n", 5, &bw);
	  size = f_size(&fp);
	  printf("bw:%d, size:%lld\n", bw, size);
	  f_close(&fp);
	#endif

  // vTaskDelete(0);

  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
}

