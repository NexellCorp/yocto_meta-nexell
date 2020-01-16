/*
 * (C) Copyright 2019 Nexell
 * Junghyun, Kim <jhkim@nexell.co.kr>
 *
 * SPDX-License-Identifier:     GPL-2.0-or-later
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <linux/version.h>
#include <sys/ioctl.h>

#include <mtd/mtd-user.h>
#include "swupdate.h"
#include "handler.h"
#include "util.h"
#include "flash.h"
#include "progress.h"

struct mtd_description {
	libmtd_t libmtd;
	struct mtd_dev_info mtd;
	bool bootsector;
};

struct mtd_ecc_bootsector {
	int sectsize;
	int datasize;
	int eccbyte;
	int eccsteps;
};

void mtd_handler(void);

enum NX_NANDC_ECC_SIZE {
	NX_NANDC_ECC_SZ_512_4 = 7,
	NX_NANDC_ECC_SZ_512_8 = 13,
	NX_NANDC_ECC_SZ_512_12 = 20,
	NX_NANDC_ECC_SZ_512_16 = 26,
	NX_NANDC_ECC_SZ_512_24 = 39,
	NX_NANDC_ECC_SZ_1024_24 = 42,
	NX_NANDC_ECC_SZ_1024_40 = 70,
	NX_NANDC_ECC_SZ_1024_60 = 105
};

#define EMPTY_BYTE	0xFF
#define isNAND(desc) \
	(desc->mtd.type == MTD_NANDFLASH || \
	 desc->mtd.type == MTD_MLCNANDFLASH)

#define NOTALIGNED_ERASE(x)	((x & (mtd->eb_size - 1)) != 0)

#define roundup(x, y) ( 			\
	{					\
        const typeof(y) __y = y;		\
        (((x) + (__y - 1)) / __y) * __y;	\
	}					\
	)

static void get_bootsector_ecc(struct mtd_dev_info *mtd,
				    struct mtd_ecc_bootsector *ecc)
{
	if (mtd->min_io_size == 512) {
		ecc->sectsize = 512;
		ecc->eccbyte = NX_NANDC_ECC_SZ_512_24;
	} else {
		ecc->sectsize = 1024;
		ecc->eccbyte = NX_NANDC_ECC_SZ_1024_60;
	}

	ecc->datasize = ecc->sectsize - ecc->eccbyte;
	ecc->datasize &= ~(1);
	ecc->eccsteps = mtd->min_io_size / ecc->sectsize;
}

/*
 * Check the bad block count from the previous bootloader
 * and get the current start offset.
 */
static int calc_bootsector_start(struct mtd_description *desc,
					  struct img_type *img)
{
	struct mtd_dev_info *mtd = &desc->mtd;
	unsigned int eb, eb_start, eb_cnt, eb_offs;
	int fd;

	fd = open(img->device, O_RDWR);
	if (fd < 0) {
		ERROR("%s: %s: %s", __func__, img->device, strerror(errno));
		return -ENODEV;
	}

	eb_start = img->seek / mtd->eb_size;
	eb_cnt = mtd->eb_cnt;

	for (eb = 0, eb_offs = -1; eb < eb_cnt; eb++) {
		if (!mtd_is_bad(mtd, fd, eb))
			eb_offs++;

		if (eb_offs == eb_start)
			break;
	}

	close(fd);

	if (eb == eb_cnt) {
		ERROR("Error: %s 0x%llx -> 0x%llx, size:0x%llx over chip:0x%x\n",
			img->fname, img->seek,
			(unsigned long long)(eb * mtd->eb_size),
			img->size, eb_cnt * mtd->eb_size);
		return -ENOSPC;
	}

	return (int)eb;
}

static int calc_bootsector_blocks(struct mtd_description *desc,
					 struct img_type *img,
					 struct mtd_ecc_bootsector *ecc)
{
	struct mtd_dev_info *mtd = &desc->mtd;
	unsigned int cnt = img->size / ecc->datasize;

	if (img->size % ecc->datasize)
		cnt += 1;

	return roundup(cnt * ecc->sectsize, mtd->eb_size) / mtd->eb_size;
}

/*
 * reference to flash_erase at 'corelib/mtd-interface.c'
 * Changed:
 * - Erase blocks as much as the image size, not full partition size
 * - If a bad block is found, erase the next block.
 */
static int mtd_erase_blocks(struct mtd_description *desc, struct img_type *img)
{
	struct mtd_dev_info *mtd = &desc->mtd;
	struct mtd_ecc_bootsector ecc;
	int fd;
	unsigned int eb, eb_start, eb_cnt, eb_end, i;
	uint8_t *buf;
	int ret = 0;

	if (!img->size)
		return -EINVAL;

	if (NOTALIGNED_ERASE(img->seek)) {
		ERROR("Non block aligned 0x%x erase offset 0x%llx",
		       mtd->eb_size, img->seek);
		return -EINVAL;
	}

	if (desc->bootsector) {
		get_bootsector_ecc(mtd, &ecc);
		eb_start = calc_bootsector_start(desc, img);
		eb_cnt = calc_bootsector_blocks(desc, img, &ecc);
	} else {
		eb_start = img->seek / mtd->eb_size;
		eb_cnt = img->size / mtd->eb_size;
		if (NOTALIGNED_ERASE(img->size))
			eb_cnt += 1;
	}

	TRACE("%s erase 0x%llx -> 0x%llx size %lld block size:%dKiB",
		img->device, img->seek,
		(unsigned long long)(eb_start * mtd->eb_size),
		img->size, mtd->eb_size / 1024);

	fd = open(img->device, O_RDWR);
	if (fd < 0) {
		ERROR("%s: %s: %s", __func__, img->device, strerror(errno));
		return -ENODEV;
	}

	/*
	 * prepare to erase all of the MTD partition,
	 */
	buf = (uint8_t *)malloc(mtd->eb_size);
	if (!buf) {
		ERROR("No memory for temporary buffer of %d bytes",
			mtd->eb_size);
		close(fd);
		return -ENOMEM;
	}

	/*
	 * Check BAD block and Unlock memory
	 */
	for (eb = eb_start, eb_end = eb_start + eb_cnt; eb < eb_end; eb++) {
		/* Always skip bad sectors */
		int isbad = mtd_is_bad(mtd, fd, eb);

		if (isbad > 0) {
			/*
			 * Add erase block count to erase next block.
			 * if erase block counts exceeds the block size of the MTD device,
			 * mtd_is_bad will be returns '-1'.
			 */
			eb_end++;
			continue;
		} else if (isbad < 0) {
			ERROR("%s: MTD get bad block %d failed", img->device, eb);
			ret  = -EFAULT;
			goto erase_out;
		}

		/* Unlock memory if required */
		if (mtd_is_locked(mtd, fd, eb) > 0) {
			if (mtd_unlock(mtd, fd, eb) != 0) {
				if (errno != EOPNOTSUPP) {
					TRACE("%s: MTD unlock block %d failure",
						img->device, eb);
					ret  = -EFAULT;
					goto erase_out;
				}
			}
		}
	}

	for (eb = eb_start, eb_end = eb_start + eb_cnt; eb < eb_end; eb++) {

		/* Always skip bad sectors */
		int isbad = mtd_is_bad(mtd, fd, eb);

		if (isbad > 0) {
			/*
			 * Add erase block count to erase next block.
			 * if erase block counts exceeds the block size of the MTD device,
			 * mtd_is_bad will be returns '-1'.
			 */
			eb_end++;
			continue;
		}

		/*
		 * In case of NOR flash, check if the flash
		 * is already empty. This can save
		 * an amount of time because erasing
		 * a NOR flash is very time expensive.
		 * NAND flash is always erased.
		 */
		if (!isNAND(desc)) {
			if (mtd_read(mtd, fd, eb, 0, buf, mtd->eb_size) != 0) {
				ERROR("%s: MTD Read block %d failure",
					img->device, eb);
				ret  = -EIO;
				goto erase_out;
			}

			/* check if already empty */
			for (i = 0; i < mtd->eb_size; i++) {
				if (buf[i] != EMPTY_BYTE)
					break;
			}

			/* skip erase if empty */
			if (i == mtd->eb_size)
				continue;

		}

		/* The sector contains data and it must be erased */
		if (mtd_erase(desc, mtd, fd, eb) != 0) {
			ERROR("%s: MTD Erase block %d failure", img->device, eb);
			ret  = -EIO;
			goto erase_out;
		}
	}

erase_out:
	free(buf);

	close(fd);

	return ret;
}

static int mtd_write_nand_bootsector(struct mtd_description *desc,
				struct img_type *img)
{
	libmtd_t *libmtd = desc->libmtd;
	struct mtd_dev_info *mtd = &desc->mtd;
	struct mtd_ecc_bootsector ecc;
	int datalen;
	bool baderaseblock = false;
	long long imglen = 0;
	long long blockstart = -1;
	long long offs;
	unsigned char *filebuf = NULL;
	long long mtdoffset = 0;
	int ifd = img->fdin;
	int fd = -1;
	bool failed = true;
	ssize_t cnt = 0;
	int ret;
	unsigned char *writebuf = NULL;
	int subpage_size = mtd->subpage_size;

	/*
	 * if nothing to do, returns without errors
	 */
	if (!img->size)
		return 0;

	get_bootsector_ecc(mtd, &ecc);

	imglen = img->size;
	mtdoffset = calc_bootsector_start(desc, img) * mtd->eb_size;
	datalen = ecc.datasize * (mtd->min_io_size/ecc.sectsize) *
		 (mtd->eb_size/mtd->min_io_size);

	TRACE("%s write to 0x%llx -> 0x%llx size %lld, bytes-block:%d",
		img->device, img->seek, mtdoffset, imglen, datalen);

	/* Flashing to NAND is currently not streamable */
	if (img->install_directly) {
		ERROR("Raw NAND not streamable");
		return -EINVAL;
	}

	filebuf = malloc(imglen);
	if (!filebuf) {
		ERROR("No memory: malloc failed for %s %lld", img->fname, imglen);
		return -ENOMEM;
	}
	writebuf = filebuf;

	if ((fd = open(img->device, O_RDWR)) < 0) {
		ERROR("%s: %s: %s", __func__, img->device, strerror(errno));
		return -ENODEV;
	}

	cnt = read(ifd, filebuf, imglen);
	if (cnt < 0) {
		ERROR("File I/O error on input");
		goto closeall;
	}

	while (writebuf < (filebuf + imglen) && mtdoffset < mtd->size) {
		int bytes = datalen;

		if (bytes > (filebuf + imglen) - writebuf)
			bytes = (filebuf + imglen) - writebuf;

		/*
		 * New eraseblock, check for bad block(s)
		 * Stay in the loop to be sure that, if mtdoffset changes because
		 * of a bad block, the next block that will be written to
		 * is also checked. Thus, we avoid errors if the block(s) after the
		 * skipped block(s) is also bad
		 */
		while (blockstart != (mtdoffset & (~mtd->eb_size + 1))) {
			blockstart = mtdoffset & (~mtd->eb_size + 1);
			offs = blockstart;

			baderaseblock = false;

			do {
				ret = mtd_is_bad(mtd, fd, offs / mtd->eb_size);
				if (ret < 0) {
					ERROR("%s: MTD get bad block failed", img->device);
					goto closeall;
				} else if (ret == 1) {
					baderaseblock = true;
				}

				if (baderaseblock) {
					mtdoffset = blockstart + mtd->eb_size;

					if (mtdoffset > mtd->size) {
						ERROR("too many bad blocks, cannot complete request");
						goto closeall;
					}
				}

				offs +=  mtd->eb_size;
			} while (offs < blockstart + mtd->eb_size);
		}

		/* Write out data */
		TRACE("write to 0x%llx %d [%s -> %s]",
			mtdoffset, bytes, img->fname, img->device);

		/* set subpate_size to avoid align check */
		mtd->subpage_size = 1;
		ret = mtd_write(libmtd, mtd, fd, mtdoffset / mtd->eb_size,
				0,
				writebuf,
				bytes,
				NULL,
				0,
				MTD_OPS_PLACE_OOB);
		mtd->subpage_size = subpage_size;

		if (ret) {
			long long i;
			if (errno != EIO) {
				ERROR("%s: MTD write failure", img->device);
				goto closeall;
			}

			for (i = blockstart; i < blockstart + mtd->eb_size; i += mtd->eb_size) {
				if (mtd_erase(libmtd, mtd, fd, i / mtd->eb_size)) {
					int errno_tmp = errno;
					TRACE("%s: MTD Erase failure", img->device);
					if (errno_tmp != EIO)
						goto closeall;
				}
			}

			TRACE("Marking block at %08llx bad",
					mtdoffset & (~mtd->eb_size + 1));
			if (mtd_mark_bad(mtd, fd, mtdoffset / mtd->eb_size)) {
				ERROR("%s: MTD Mark bad block failure", img->device);
				goto closeall;
			}
			mtdoffset = blockstart + mtd->eb_size;

			continue;
		}

		/*
		 * this handler does not use copyfile()
		 * and must update itself the progress bar
		 */
		swupdate_progress_update((img->size - imglen) * 100 / img->size);

		mtdoffset += mtd->eb_size;
		writebuf += bytes;
	}
	failed = false;

closeall:
	free(filebuf);
	close(fd);

	if (failed) {
		ERROR("Installing image %s into %s failed",
			img->fname, img->device);
		return -1;
	}

	return 0;
}

/* Check whether buffer is filled with character 'pattern' */
static inline int buffer_check_pattern(unsigned char *buffer, size_t size,
					unsigned char pattern)
{
	/* Invalid input */
	if (!buffer || (size == 0))
		return 0;

	/* No match on first byte */
	if (*buffer != pattern)
		return 0;

	/* First byte matched and buffer is 1 byte long, OK. */
	if (size == 1)
		return 1;

	/*
	 * Check buffer longer than 1 byte. We already know that buffer[0]
	 * matches the pattern, so the test below only checks whether the
	 * buffer[0...size-2] == buffer[1...size-1] , which is a test for
	 * whether the buffer is filled with constant value.
	 */
	return !memcmp(buffer, buffer + 1, size - 1);
}

/*
 * Writing to the NAND must take into account ECC errors
 * and BAD sectors.
 * This is not required for NOR flashes
 * The function reassembles nandwrite from mtd-utils
 * dropping all options that are not required here.
 */
static void erase_buffer(void *buffer, size_t size)
{
	const uint8_t kEraseByte = 0xff;

	if (buffer != NULL && size > 0)
		memset(buffer, kEraseByte, size);
}

/*
 * reference to flash_write_nand at 'handlers/flash_handler.c'
 */
static int mtd_write_nand(struct mtd_description *desc, struct img_type *img)
{
	libmtd_t *libmtd = desc->libmtd;
	struct mtd_dev_info *mtd = &desc->mtd;
	int pagelen;
	bool baderaseblock = false;
	long long imglen = 0;
	long long blockstart = -1;
	long long offs;
	unsigned char *filebuf = NULL;
	size_t filebuf_max = 0;
	size_t filebuf_len = 0;
	long long mtdoffset = 0;
	int ifd = img->fdin;
	int fd = -1;
	bool failed = true;
	int ret;
	unsigned char *writebuf = NULL;

	/*
	 * if nothing to do, returns without errors
	 */
	if (!img->size)
		return 0;

	pagelen = mtd->min_io_size;
	imglen = img->size;
	mtdoffset = img->seek;

	if ((imglen / pagelen) * mtd->min_io_size > mtd->size) {
		ERROR("Image %s does not fit into %s", img->fname, img->device);
		return -EIO;
	}

	/* Flashing to NAND is currently not streamable */
	if (img->install_directly) {
		ERROR("Raw NAND not streamable");
		return -EINVAL;
	}

	filebuf_max = mtd->eb_size / mtd->min_io_size * pagelen;
	filebuf = calloc(1, filebuf_max);
	erase_buffer(filebuf, filebuf_max);

	if ((fd = open(img->device, O_RDWR)) < 0) {
		ERROR("%s: %s: %s", __func__, img->device, strerror(errno));
		return -ENODEV;
	}

	/*
	 * Get data from input and write to the device while there is
	 * still input to read and we are still within the device
	 * bounds. Note that in the case of standard input, the input
	 * length is simply a quasi-boolean flag whose values are page
	 * length or zero.
	 */
	while ((imglen > 0 || writebuf < filebuf + filebuf_len)
		&& mtdoffset < mtd->size) {
		/*
		 * New eraseblock, check for bad block(s)
		 * Stay in the loop to be sure that, if mtdoffset changes because
		 * of a bad block, the next block that will be written to
		 * is also checked. Thus, we avoid errors if the block(s) after the
		 * skipped block(s) is also bad
		 */
		while (blockstart != (mtdoffset & (~mtd->eb_size + 1))) {
			blockstart = mtdoffset & (~mtd->eb_size + 1);
			offs = blockstart;

			/*
			 * if writebuf == filebuf, we are rewinding so we must
			 * not reset the buffer but just replay it
			 */
			if (writebuf != filebuf) {
				erase_buffer(filebuf, filebuf_len);
				filebuf_len = 0;
				writebuf = filebuf;
			}

			baderaseblock = false;

			do {
				ret = mtd_is_bad(mtd, fd, offs / mtd->eb_size);
				if (ret < 0) {
					ERROR("%s: MTD get bad block failed", img->device);
					goto closeall;
				} else if (ret == 1) {
					baderaseblock = true;
				}

				if (baderaseblock) {
					mtdoffset = blockstart + mtd->eb_size;

					if (mtdoffset > mtd->size) {
						ERROR("too many bad blocks, cannot complete request");
						goto closeall;
					}
				}

				offs +=  mtd->eb_size;
			} while (offs < blockstart + mtd->eb_size);
		}

		/* Read more data from the input if there isn't enough in the buffer */
		if (writebuf + mtd->min_io_size > filebuf + filebuf_len) {
			size_t readlen = mtd->min_io_size;
			size_t alreadyread = (filebuf + filebuf_len) - writebuf;
			size_t tinycnt = alreadyread;
			ssize_t cnt = 0;

			while (tinycnt < readlen) {
				cnt = read(ifd, writebuf + tinycnt, readlen - tinycnt);
				if (cnt == 0) { /* EOF */
					break;
				} else if (cnt < 0) {
					ERROR("File I/O error on input");
					goto closeall;
				}
				tinycnt += cnt;
			}

			/* No padding needed - we are done */
			if (tinycnt == 0) {
				imglen = 0;
				break;
			}

			/* Padding */
			if (tinycnt < readlen) {
				erase_buffer(writebuf + tinycnt, readlen - tinycnt);
			}

			filebuf_len += readlen - alreadyread;

			imglen -= tinycnt - alreadyread;

		}

		ret = 0;
		if (!buffer_check_pattern(writebuf, mtd->min_io_size, 0xff)) {
			/* Write out data */
			ret = mtd_write(libmtd, mtd, fd, mtdoffset / mtd->eb_size,
					mtdoffset % mtd->eb_size,
					writebuf,
					mtd->min_io_size,
					NULL,
					0,
					MTD_OPS_PLACE_OOB);
		}
		if (ret) {
			long long i;
			if (errno != EIO) {
				ERROR("%s: MTD write failure", img->device);
				goto closeall;
			}

			/* Must rewind to blockstart if we can */
			writebuf = filebuf;

			for (i = blockstart; i < blockstart + mtd->eb_size; i += mtd->eb_size) {
				if (mtd_erase(libmtd, mtd, fd, i / mtd->eb_size)) {
					int errno_tmp = errno;
					TRACE("%s: MTD Erase failure", img->device);
					if (errno_tmp != EIO)
						goto closeall;
				}
			}

			TRACE("Marking block at %08llx bad",
					mtdoffset & (~mtd->eb_size + 1));
			if (mtd_mark_bad(mtd, fd, mtdoffset / mtd->eb_size)) {
				ERROR("%s: MTD Mark bad block failure", img->device);
				goto closeall;
			}
			mtdoffset = blockstart + mtd->eb_size;

			continue;
		}

		/*
		 * this handler does not use copyfile()
		 * and must update itself the progress bar
		 */
		swupdate_progress_update((img->size - imglen) * 100 / img->size);

		mtdoffset += mtd->min_io_size;
		writebuf += pagelen;
	}
	failed = false;

closeall:
	free(filebuf);
	close(fd);

	if (failed) {
		ERROR("Installing image %s into %s failed",
			img->fname, img->device);
		return -1;
	}

	return 0;
}

static int mtd_write_nor(struct mtd_description *desc,
			       struct img_type *img)
{
	libmtd_t *libmtd = desc->libmtd;
	struct mtd_dev_info *mtd = &desc->mtd;
	int fdout;
	int ret;

	if  (!mtd_dev_present(libmtd, mtd->mtd_num)) {
		ERROR("MTD %d does not exist", mtd->mtd_num);
		return -ENODEV;
	}

	if ((fdout = open(img->device, O_RDWR)) < 0) {
		ERROR("%s: %s: %s", __func__, img->device, strerror(errno));
		return -1;
	}

	ret = copyimage(&fdout, img, NULL);

	/* tell 'nbytes == 0' (EOF) from 'nbytes < 0' (read error) */
	if (ret < 0) {
		ERROR("Failure installing into: %s", img->device);
		return -1;
	}
	close(fdout);
	return 0;
}

static int mtd_write_image(struct mtd_description *desc, struct img_type *img)
{
	if (isNAND(desc)) {
		if (desc->bootsector)
			return mtd_write_nand_bootsector(desc, img);
		else
			return mtd_write_nand(desc, img);
	} else
		return mtd_write_nor(desc, img);
}

static int mtd_install_image(struct img_type *img, bool bootsector)
{
	struct mtd_description desc = { .bootsector = bootsector };
	int fd;

	desc.libmtd = libmtd_open();
	if (desc.libmtd == NULL) {
		ERROR("can't initialize libmtd");
		return -1;
	}

	fd = open(img->device, O_RDWR);
	if (fd < 0) {
		ERROR("Wrong MTD device in description: %s", img->device);
		return -1;
	}

	if (mtd_get_dev_info(desc.libmtd, img->device, &desc.mtd) < 0) {
		ERROR("mtd_get_dev_info:%s failed", img->device);
		return -1;
	}
	close(fd);

	if (mtd_erase_blocks(&desc, img)) {
		ERROR("I cannot erasing %s offset:0x%llx size:0x%llx",
		      img->device, img->seek, img->partsize);
		return -1;
	}

	TRACE("Copying %s into %s", img->fname, img->device);
	if (mtd_write_image(&desc, img)) {
		ERROR("I cannot copy %s into %s offset:0x%llx size:0x%llx",
			img->fname, img->device, img->seek, img->partsize);
		return -1;
	}

	return 0;
}

static int install_mtd_image(struct img_type *img,
				   void __attribute__ ((__unused__)) *data)
{
	TRACE("MTD type: %s %s: 0x%llx:0x%llx -> %s",
	      img->type, img->fname, img->seek, img->size, img->device);

	return mtd_install_image(img, false);
}

static int install_mtd_bootsector(struct img_type *img,
				   void __attribute__ ((__unused__)) *data)
{
	TRACE("MTD type: %s %s: 0x%llx:0x%llx -> %s",
	      img->type, img->fname, img->seek, img->size, img->device);

	return mtd_install_image(img, true);
}

__attribute__((constructor))
void mtd_handler(void)
{
	register_handler("mtd-bootsector", install_mtd_bootsector,
				IMAGE_HANDLER | FILE_HANDLER, NULL);
	register_handler("mtd", install_mtd_image,
				IMAGE_HANDLER | FILE_HANDLER, NULL);
}
