/*
 * retarget.c
 *
 *  Created on: Feb 15, 2020
 *      Author: icis4
 */


#include <stdio.h>
#include "main.h"
#include "usart.h"
#include "lcd_log/lcd_log.h"

int _write(int file, char *ptr, int len)
{
	switch(file) {
	default:
		//HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 100);
		for (int i = 0; i < len; i++)
		{
			__io_putchar(ptr[i]);
		}
		break;
	}

	return len;
}

