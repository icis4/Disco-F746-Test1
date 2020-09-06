/**
  ******************************************************************************
  * @file   fatfs.h
  * @brief  Header for fatfs applications
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "ff.h"
#include "ff_gen_drv.h"
#include "sdram_diskio.h" /* defines SDRAMDISK_Driver as external */
#include "sd_diskio.h" /* defines SD_Driver as external */
#include "user_diskio.h" /* defines USER_Driver as external */

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern uint8_t retSDRAMDISK; /* Return value for SDRAMDISK */
extern char SDRAMDISKPath[4]; /* SDRAMDISK logical drive path */
extern FATFS SDRAMDISKFatFS; /* File system object for SDRAMDISK logical drive */
extern FIL SDRAMDISKFile; /* File object for SDRAMDISK */
extern uint8_t retSD; /* Return value for SD */
extern char SDPath[4]; /* SD logical drive path */
extern FATFS SDFatFS; /* File system object for SD logical drive */
extern FIL SDFile; /* File object for SD */
extern uint8_t retUSER; /* Return value for USER */
extern char USERPath[4]; /* USER logical drive path */
extern FATFS USERFatFS; /* File system object for USER logical drive */
extern FIL USERFile; /* File object for USER */

void MX_FATFS_Init(void);

/* USER CODE BEGIN Prototypes */
// http://elm-chan.org/fsw/ff/doc/sfileinfo.html
// fdate    - The date when the file was modified or the directory was created.
// bit15:9  - Year origin from 1980 (0..127)
// bit8:5   - Month (1..12)
// bit4:0   - Day (1..31)
// ftime    - The time when the file was modified or the directory was created.
// bit15:11 - Hour (0..23)
// bit10:5  - Minute (0..59)
// bit4:0   - Second / 2 (0..29)

typedef union FAT_Date_Time_Type {
	DWORD ftime;
	struct {
		WORD time;
		WORD date;
	};
	struct {
		unsigned int Seconds:5;
		unsigned int Minutes:6;
		unsigned int Hours:5;
		unsigned int Day:5;
		unsigned int Month:4;
		unsigned int Year:7;
	};
} FAT_Date_Time_Type;

/* USER CODE END Prototypes */
#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
