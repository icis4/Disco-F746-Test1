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
#include "bsp/stm32746g_discovery_lcd.h"
#include "bsp/fonts/fonts.h"
#include "lcd_log/lcd_log.h"

uint8_t resultQSPI;

extern void MX_USB_DEVICE_Init(void);
extern void MX_FATFS_Init(void);

result_t InitApplication(void)
{
	/* init code for USB_HOST */

	configureTimerForDelay_us();
	resultQSPI = BSP_QSPI_Init();
	// MX_FATFS_Init();

	BSP_LCD_Init();
	BSP_LCD_LayerRgb565Init(0, LCD_FB_START_ADDRESS);
	BSP_LCD_DisplayOn();
	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_BLUE);
//	BSP_LCD_SetTransparency(0, 100);
//	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
//	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
//	BSP_LCD_SetFont(&Font16);
//	BSP_LCD_DisplayStringAtLine(0, "Baf Maamu!");

	LCD_LOG_Init();
	LCD_LOG_SetHeader("STM32F746G-DISCO");
	LCD_LOG_SetFooter("(c) 2020 Ivaylo Ilchev");

	return HAL_OK;
}
