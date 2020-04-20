/* Includes ------------------------------------------------------------------*/
#include <flash_disk/bsp_driver_flash.h>
#include <bsp/stm32746g_discovery_qspi.h>

uint8_t BSP_FLASH_Init(void)
{
  uint8_t flashstatus = FLASH_OK;

  if (BSP_QSPI_Init() != QSPI_OK) {
	    flashstatus = FLASH_ERROR;
  }
  
  return flashstatus;
}

uint8_t BSP_FLASH_ReadData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{ 
  uint8_t flashstatus = FLASH_OK;
  
  if(BSP_QSPI_Read((uint8_t*)pData, uwStartAddress, uwDataSize) != QSPI_OK)
  {
    flashstatus = FLASH_ERROR;
  }

  return flashstatus;
}

uint8_t BSP_FLASH_WriteData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{ 
  uint8_t flashstatus = FLASH_OK;
  
  BSP_QSPI_Erase_Block(uwStartAddress);

  if(BSP_QSPI_Write((uint8_t*)pData, uwStartAddress, uwDataSize) != QSPI_OK)
  {
    flashstatus = FLASH_ERROR;
  }

  return flashstatus;
}
