/*
 * debug.h
 *
 *  Created on: 2.09.2020
 *      Author: Ivaylo Ilchev ii@melexis.com
 */

#ifndef APPLICATION_DEBUG_H_
#define APPLICATION_DEBUG_H_

#if defined(DEBUG)
#define DBG_PRINT(...) printf(__VA_ARGS__)
#else
#define DBG_PRINT(...)
#endif


#endif /* APPLICATION_DEBUG_H_ */
