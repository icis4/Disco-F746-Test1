/*
 * init_application.c
 *
 *  Created on: Feb 14, 2020
 *      Author: icis4
 */
#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app/application.h"
#include "app/tools.h"
#include "bsp/stm32746g_discovery_qspi.h"

uint8_t resultQSPI;

extern void MX_USB_DEVICE_Init(void);
extern void MX_FATFS_Init(void);

result_t InitApplication(void)
{
	/* init code for USB_HOST */

	configureTimerForDelay_us();
	resultQSPI = BSP_QSPI_Init();
	// MX_FATFS_Init();

	return HAL_OK;
}
