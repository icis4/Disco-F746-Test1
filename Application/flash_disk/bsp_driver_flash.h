#ifndef __STM32F7_FLASH_H
#define __STM32F7_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sdram.h"

#define   FLASH_OK         ((uint8_t)0x00)
#define   FLASH_ERROR      ((uint8_t)0x01)

#define FLASH_DEVICE_SIZE  ((uint32_t)0x800000)  /* SDRAM device size in MBytes */
#define FLASH_TIMEOUT     ((uint32_t)0xFFFF)
   
/* Exported functions --------------------------------------------------------*/   
uint8_t BSP_FLASH_Init(void);
uint8_t BSP_FLASH_ReadData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t BSP_FLASH_WriteData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F7_FLASH_H */
