/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "fatfs.h"

uint8_t retSDRAMDISK;    /* Return value for SDRAMDISK */
char SDRAMDISKPath[4];   /* SDRAMDISK logical drive path */
FATFS SDRAMDISKFatFS;    /* File system object for SDRAMDISK logical drive */
FIL SDRAMDISKFile;       /* File object for SDRAMDISK */
uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */
uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
#include <stdlib.h>
#include <stdio.h>
#include "rtc.h"

uint8_t workBuffer[_MAX_SS];

extern int diskfree(char* path, DWORD *total_kb, DWORD *free_kb);

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SDRAMDISK driver ###########################*/
  retSDRAMDISK = FATFS_LinkDriver(&SDRAMDISK_Driver, SDRAMDISKPath);
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  FATFS* fs;
  fs = malloc(sizeof(FATFS));
  f_mount(fs, "", 0); /* Mount default drive */
  f_mount(NULL, "", 0); /* Unmount default drive */
  free(fs);

  retSDRAMDISK = f_mount(&SDRAMDISKFatFS, SDRAMDISKPath, 0);
  if (retSDRAMDISK == FR_OK)
	  retSDRAMDISK = f_mkfs(SDRAMDISKPath, FM_ANY, 0, workBuffer, sizeof(workBuffer));

  retSD = f_mount(&SDFatFS, SDPath, 0);
  if (retSD == FR_OK && 0) {
	  printf("\nFormat %s ...", SDPath);
	  retSD = f_mkfs(SDPath, FM_ANY, 0, workBuffer, sizeof(workBuffer));
	  printf("%s\n", (retSD)? "Fail":"Done");
  }

  retUSER = f_mount(&USERFatFS, USERPath, 1);
  if (retUSER != FR_OK && 1) {
	  printf("\nFormat %s ...", USERPath);
	  retUSER = f_mkfs(USERPath, FM_ANY, 0, workBuffer, sizeof(workBuffer));
	  printf("%s\n", (retUSER)? "Fail":"Done");
  }

  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	FAT_Date_Time_Type dt;

	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	dt.Year = (date.Year >= 1980) ? date.Year - 1980 : date.Year;
	dt.Month = date.Month;
	dt.Day = date.Date;

	dt.Hours = time.Hours;
	dt.Minutes = time.Minutes;
	dt.Seconds = time.Seconds;

	return dt.ftime;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
