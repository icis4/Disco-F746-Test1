/*
 * config.h
 *
 *  Created on: 27.04.2017
 *      Author: Ivaylo Ilchev iic@melexis.com
 */

#ifndef CONFIG_H_
#define CONFIG_H_

extern const char* version_str;

#if !defined(CFG_VERSION)
#  define CFG_VERSION "<undefined>"
#endif

#if !defined(GIT_BRANCH)
#  define GIT_BRANCH "<undefined>"
#endif

#if !defined(CFG_BUILD)
#  define CFG_BUILD "<undefined>"
#endif

#if !defined(CFG_DATE)
#  define CFG_DATE "<undefined>"
#endif

#endif /* CONFIG_H_ */
