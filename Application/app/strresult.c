/*
 * strresult.c
 *
 *  Created on: 20.04.2020 г.
 *      Author: icis4
 */
#include <stdio.h>
#include "main.h"

const char* strresult(uint32_t result)
{
	static char* buff = "ERR:0000";
	  switch(result) {
	  case HAL_OK:
		  return "OK";
		  break;
	  case HAL_ERROR:
		  return "ERROR";
		  break;
	  case HAL_BUSY:
		  return "BUSY";
		  break;
	  case HAL_TIMEOUT:
		  return "TIMEOUT";
		  break;
	  default:
		  sprintf(buff, "ERR:%04x", (unsigned short)result);
		  return buff;
		  break;
	  }
}
