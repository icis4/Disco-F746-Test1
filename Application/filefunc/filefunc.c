/*
 * fatfs.c
 *
 *  Created on: 5.05.2020 г.
 *      Author: icis4
 */
/* FatFs includes component */
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "ff_gen_drv.h"
#include "fatfs.h"

#define __ELASTERROR 2000	/* Users can add values starting here */

int diskfree(DWORD *total_kb, DWORD *free_kb)
{
	FATFS *fs = &SDFatFS;
	DWORD fre_clust, fre_sect, tot_sect;
	FRESULT result;

	/* Get volume information and free clusters of drive 1 */
	result = f_getfree("1:", &fre_clust, &fs);
	if (result != FR_OK) {
		errno = __ELASTERROR + result;
		return -1;
	}

	/* Get total sectors and free sectors */
	tot_sect = (fs->n_fatent - 2) * fs->csize;
	fre_sect = fre_clust * fs->csize;

	/* Print the free space (assuming 512 bytes/sector) */
	*total_kb = tot_sect / 2;
	*free_kb = fre_sect / 2;

	return 0;
}

