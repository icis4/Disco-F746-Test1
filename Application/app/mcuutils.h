/*
 * mcuutils.h
 *
 *  Created on: 21.08.2016 �.
 *      Author: ivaylo
 */

#ifndef MCUUTILS_H_
#define MCUUTILS_H_

#include "stm32f7xx_hal.h"

/* MCU Factory programmed flash size in Kbytes, uint16_t */
#define FLASHSIZE (*(__IO uint16_t *)(FLASHSIZE_BASE))

/*
MCU Package data register:
Bits 15:11 Reserved, must be kept at reset value.
Bits 10:8 PKG[2:0]: Package type
	0x011: LQFP144, UFBGA144 (7x7) and UFBGA144 (10x10) packages
	0x010: WLCSP81 package
	0x001: LQFP100 package
	0x000: LQFP64 package
Bits 7:0 Reserved, must be kept at reset value.
*/
#define MCUPKG ((((*(__IO uint16_t *)(PACKAGE_BASE))) >> 8) & 7)

extern uint8_t MCU_ID[];
uint8_t* mcu_id(void);

#endif /* MCUUTILS_H_ */
