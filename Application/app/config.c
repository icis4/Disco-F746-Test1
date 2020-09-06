/*
 * configure.c
 *
 *  Created on: 27.04.2017
 *      Author: Ivaylo Ilchev iic@melexis.com
 */

#include "main.h"
#include "stm32f7xx_hal.h"
#include "app/config.h"

const char* version_str = "DISCO-746G ver:" CFG_VERSION "\n" GIT_BRANCH " " CFG_BUILD " " CFG_DATE "\n(c) Ivaylo Ilchev 2020\n";
