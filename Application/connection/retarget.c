/*
 * retarget.c
 *
 *  Created on: Feb 15, 2020
 *      Author: icis4
 */


#include <stdio.h>
#include "main.h"
#include "usart.h"

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 100);

	return len;
}

