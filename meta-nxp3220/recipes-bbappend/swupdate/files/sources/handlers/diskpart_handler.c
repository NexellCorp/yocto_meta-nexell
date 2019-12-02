/*
 * (C) Copyright 2019 Nexell
 * Junghyun, Kim <jhkim@nexell.co.kr>
 *
 * SPDX-License-Identifier:     GPL-2.0-or-later
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

#include "swupdate.h"
#include "handler.h"
#include "util.h"

#define	BLOCK_SIZE	512

void disk_part_handler(void);

static int disk_part_create(const char *type, struct img_type *img)
{
	static bool disk_part_created = false;
	const char *prog = "parted";
	char cmd[1024] = { 0, };
	off64_t blk_start, blk_end;
	int ret = 0;

	if (!disk_part_created) {
		snprintf(cmd, sizeof(cmd),
			"%s %s --script -- unit s mklabel %s",
			prog, img->device, type);
		TRACE("disk part: %s", cmd);

		ret = system(cmd);
		if (ret) {
			ERROR("Failed %s: %s\n", cmd, strerror(errno));
			return ret;
		}
		disk_part_created = true;
	}

	blk_start = img->seek / BLOCK_SIZE;
	blk_end = img->partsize ?
		  ((img->seek + img->partsize) / BLOCK_SIZE) - 1: 0;

	/*
         * ¡®s¡¯ : sector (n bytes depending on the sector size, often 512)
         * ¡®B¡¯ : byte
         */
	if (blk_end)
		snprintf(cmd, sizeof(cmd),
			"%s %s --script -- unit s mkpart primary %lld %lld",
			prog, img->device, blk_start, blk_end);
	else
		snprintf(cmd, sizeof(cmd),
			"%s %s --script -- unit s mkpart primary %lld %s",
			prog, img->device, blk_start, "100%");

	TRACE("make part: %s", cmd);

	ret = system(cmd);
	if (ret)
		ERROR("Failed %s: %s\n", cmd, strerror(errno));

	return ret;
}

static int install_gpt_part(struct img_type *img,
			void __attribute__ ((__unused__)) *data)
{
	TRACE("Volume %s: %s: 0x%llx:0x%llx -> %s",
		img->type, img->volname, img->seek, img->partsize, img->device);

	return disk_part_create("gpt", img);
}

static int install_dos_part(struct img_type *img,
			void __attribute__ ((__unused__)) *data)
{
	TRACE("Volume %s: %s: 0x%llx:0x%llx -> %s",
		img->type, img->volname, img->seek, img->partsize, img->device);

	return disk_part_create("msdos", img);
}

__attribute__((constructor))
void disk_part_handler(void)
{
	register_handler("gpt", install_gpt_part, PARTITION_HANDLER, NULL);
	register_handler("dos", install_dos_part, PARTITION_HANDLER, NULL);
}
