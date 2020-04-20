/*
 * tools.h
 *
 *  Created on: 20.04.2020 г.
 *      Author: icis4
 */

#ifndef APP_TOOLS_H_
#define APP_TOOLS_H_
#ifdef __cplusplus
 extern "C" {
#endif

const char* strresult(uint32_t result);

extern void configureTimerForDelay_us(void);
extern void BSP_DelayMicros(uint32_t microseconds);
extern uint32_t BSP_GetMicros(void);

#ifdef __cplusplus
 }
#endif /* __cplusplus */
#endif /* APP_TOOLS_H_ */
