/*
 * application.h
 *
 *  Created on: Feb 14, 2020
 *      Author: icis4
 */

#ifndef APP_APPLICATION_H_
#define APP_APPLICATION_H_

typedef uint32_t result_t;

#define NOERR 0

#define CHK_ERR(func) do { result_t _$_result = func; if (_$_result) return _$_result; } while(0);
#define CHK_ERRNO(func) do { errno = 0; result_t _$e_result = func; if (_$e_result != 0) return errno + 0x100; } while(0);

extern result_t InitApplication(void);

#endif /* APP_APPLICATION_H_ */
