/*
 * termios.c
 *
 *  Created on: 26.04.2020 г.
 *      Author: icis4
 */
#include "termios.h"

int tcgetattr(int fd, struct termios *termios_p)
{
	return 0;
}

int tcsetattr(int fd, int optional_actions,
              const struct termios *termios_p)
{
	return 0;
}
