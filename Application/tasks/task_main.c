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

__NO_RETURN void StartDefaultTask(void *argument)
{
  InitApplication();
  printf("\n*** Ready ***\n");

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
}

