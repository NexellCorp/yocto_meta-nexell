/*
 * (C) Copyright 2019 Nexell
 * Junghyun, Kim <jhkim@nexell.co.kr>
 *
 * SPDX-License-Identifier:     GPL-2.0-or-later
 */
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <libgen.h>

#include "swupdate.h"
#include "handler.h"
#include "util.h"
#include "progress.h"

#include "libsparse/include/sparse/sparse.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

void android_sparse_image_handler(void);

static int android_sparse_image(struct img_type *img,
				void __attribute__ ((__unused__)) *data)
{
	struct sparse_file *s = NULL;
	char *filein, *fileout;
	int fdin, fdout;
	off64_t seek;
	int ret;

	fdin = img->fdin;
	filein = img->fname;
	fileout = img->device;
	seek = img->seek;

	TRACE("Installing file %s:0x%llx on %s:0x%llx",
		filein, lseek(fdin, 0, SEEK_CUR), fileout, seek);

	fdout = open(fileout, O_RDWR | O_CREAT | O_BINARY, 0644);
	if (fdout < 0) {
		ERROR("Device %s cannot be opened: %s",
			fileout, strerror(errno));
		return -1;
	}
	lseek(fdout, 0, SEEK_SET);

	/*
	 * Replace input file start offset for lseek
	 */
	sparse_file_lseek_offset(lseek(fdin, 0, SEEK_CUR));

	s = sparse_file_import(fdin, true, false);
	if (!s) {
		ERROR("File %s cannot read :%s", filein, strerror(errno));
		return -1;
	}

	if (seek != lseek(fdout, seek, SEEK_CUR)) {
		ERROR("Device %s cannot lseek 0x%llx: %s",
			fileout, seek, strerror(errno));
		return -1;
	}

	ret = sparse_file_write(s, fdout, false, false, false,
				swupdate_progress_update);
	if (ret < 0) {
		ERROR("Device %s cannot write: %s", fileout, strerror(errno));
		return -1;
	}

	lseek(fdout, 0, SEEK_SET);
	sparse_file_destroy(s);

	close(fdout);

	fprintf(stdout, "ok\n");

	return 0;
}

__attribute__((constructor))
void android_sparse_image_handler(void)
{
	register_handler("android-sparse",
			android_sparse_image, IMAGE_HANDLER, NULL);
}
