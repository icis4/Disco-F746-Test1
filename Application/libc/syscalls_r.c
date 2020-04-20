/*
 * syscalls_r.c
 *
 *  Created on: 16.04.2020 г.
 *      Author: icis4
 */
#include <stdio.h>

int _open_r(void *reent, const char *file, int flags, int mode);
int _close_r(void *reent, int fd);
off_t _lseek_r(void *reent, int fd, off_t pos, int whence);
long _read_r(void *reent, int fd, void *buf, size_t cnt);
long _write_r(void *reent, int fd, const void *buf, size_t cnt);
int _fork_r(void *reent);
int _wait_r(void *reent, int *status);
int _stat_r(void *reent, const char *file, struct stat *pstat);
int _fstat_r(void *reent, int fd, struct stat *pstat);
int _link_r(void *reent, const char *old, const char *new);
int _unlink_r(void *reent, const char *file);
char* _sbrk_r(void *reent, size_t incr);

