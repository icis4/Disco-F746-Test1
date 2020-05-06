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

int diskfree(char* path, DWORD *total_kb, DWORD *free_kb)
{
	FATFS *fs;
	DWORD fre_clust, fre_sect, tot_sect;
	FRESULT result;


	/* Get volume information and free clusters of drive 1 */
	if (*((uint16_t*)SDPath) == *((uint16_t*)path)) {
		fs = &SDFatFS;
		result = f_getfree(SDPath, &fre_clust, &fs);

		/* Get total sectors and free sectors */
		tot_sect = (fs->n_fatent - 2) * fs->csize;
		fre_sect = fre_clust * fs->csize;

		/* Print the free space (assuming 512 bytes/sector) */
		*total_kb = tot_sect / 2;
		*free_kb = fre_sect / 2;

	} else if (*((uint16_t*)SDRAMDISKPath) == *((uint16_t*)path)) {
		fs = &SDRAMDISKFatFS;
		result = f_getfree(SDRAMDISKPath, &fre_clust, &fs);

		/* Get total sectors and free sectors */
		tot_sect = (fs->n_fatent - 2) * fs->csize;
		fre_sect = fre_clust * fs->csize;

		/* Print the free space (assuming 512 bytes/sector) */
		*total_kb = tot_sect / 2;
		*free_kb = fre_sect / 2;
	} else if (*((uint16_t*)USERPath) == *((uint16_t*)path)) {
		fs = &USERFatFS;
		result = f_getfree(USERPath, &fre_clust, &fs);

		/* Get total sectors and free sectors */
		tot_sect = (fs->n_fatent - 2) * fs->csize;
		fre_sect = fre_clust * fs->csize;

		/* Print the free space (assuming 4096 bytes/sector) */
		*total_kb = tot_sect * 4;
		*free_kb = fre_sect * 4;

	} else {
		*total_kb = -1;
		*free_kb = -1;
		errno = __ELASTERROR + FR_EXIST;
		return -1;
	}

	if (result != FR_OK) {
		errno = __ELASTERROR + result;
		return -1;
	}

	return 0;
}

