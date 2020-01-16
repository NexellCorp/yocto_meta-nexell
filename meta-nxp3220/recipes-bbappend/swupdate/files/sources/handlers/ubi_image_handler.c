/*
 * (C) Copyright 2019 Nexell
 * Junghyun, Kim <jhkim@nexell.co.kr>
 *
 * SPDX-License-Identifier:     GPL-2.0-or-later
 */
#define MAX_CONSECUTIVE_BAD_BLOCKS 4

#define PROGRAM_NAME    "ubiformat"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>

#include <mtd/mtd-user.h>
#include "swupdate.h"
#include "handler.h"
#include "util.h"
#include "flash.h"
#include "progress.h"

#include "libmtd/common.h"
#include "libmtd/libubigen.h"
#include "libmtd/libscan.h"
#include "libmtd/mtd_swab.h"
#include "libmtd/crc32.h"

/* The variables below are set by command line arguments */
struct args {
	unsigned int yes:1;
	unsigned int quiet:1;
	unsigned int verbose:1;
	unsigned int override_ec:1;
	unsigned int novtbl:1;
	unsigned int manual_subpage;
	int subpage_size;
	int vid_hdr_offs;
	int ubi_ver;
	uint32_t image_seq;
	off_t image_sz;
	long long ec;
	const char *image;
	const char *node;
	int node_fd;
};

struct argopts {
	bool want_exit;
	bool answer_is_yes;
};

static struct args args = {
	.ubi_ver = 1,
};

static struct argopts opts = {
	.want_exit = false,
	.answer_is_yes = true,
};

void ubi_image_handler(void);

static long long get_bytes(const char *str)
{
	char *endp;
	long long bytes = strtoull(str, &endp, 0);

	if (bytes < 0) {
		ERROR("incorrect amount of bytes: \"%s\"", str);
		return -1;
	}

	return bytes;
}

/*
 * reference to ubi_format at mtd-utils's ubi-utils
 */
static int parse_opt(struct img_type *img)
{
	if (!img->device)
		return -EINVAL;

	/* set default args */
	util_srand();

	args.override_ec = 0;
	args.ec = 0;
	args.ubi_ver = 1;
	args.image_seq = rand();
	args.yes = 1;

	args.quiet = false;
	args.verbose = 0;

	/* set args with swupdate arguments */
	if (img->type_data)
		args.subpage_size = get_bytes(img->type_data);

	args.node = img->device;
	args.image = img->fname;

	return 0;
}

static int want_exit(void)
{
	return opts.want_exit == true ? 0 : 1;
}

static int answer_is_yes(const char __attribute__ ((__unused__)) *msg)
{
	return opts.answer_is_yes == true ? 1 : 0;
}

static void print_bad_eraseblocks(const struct mtd_dev_info *mtd,
				  const struct ubi_scan_info *si)
{
	int first = 1, eb;

	if (si->bad_cnt == 0)
		return;

	normsg_cont("%d bad eraseblocks found, numbers: ", si->bad_cnt);
	for (eb = 0; eb < mtd->eb_cnt; eb++) {
		if (si->ec[eb] != EB_BAD)
			continue;
		if (first) {
			printf("%d", eb);
			first = 0;
		} else
			printf(", %d", eb);
	}
	printf("\n");
}

static int change_ech(struct ubi_ec_hdr *hdr, uint32_t image_seq,
		      long long ec)
{
	uint32_t crc;

	/* Check the EC header */
	if (be32_to_cpu(hdr->magic) != UBI_EC_HDR_MAGIC)
		return errmsg("bad UBI magic %#08x, should be %#08x",
			      be32_to_cpu(hdr->magic), UBI_EC_HDR_MAGIC);

	crc = mtd_crc32(UBI_CRC32_INIT, hdr, UBI_EC_HDR_SIZE_CRC);
	if (be32_to_cpu(hdr->hdr_crc) != crc)
		return errmsg("bad CRC %#08x, should be %#08x\n",
			      crc, be32_to_cpu(hdr->hdr_crc));

	hdr->image_seq = cpu_to_be32(image_seq);
	hdr->ec = cpu_to_be64(ec);
	crc = mtd_crc32(UBI_CRC32_INIT, hdr, UBI_EC_HDR_SIZE_CRC);
	hdr->hdr_crc = cpu_to_be32(crc);

	return 0;
}

static int drop_ffs(const struct mtd_dev_info *mtd, const void *buf, int len)
{
	int i;

	for (i = len - 1; i >= 0; i--)
		if (((const uint8_t *)buf)[i] != 0xFF)
			break;

	/* The resulting length must be aligned to the minimum flash I/O size */
	len = i + 1;
	len = (len + mtd->min_io_size - 1) / mtd->min_io_size;
	len *=  mtd->min_io_size;
	return len;
}

static int read_all(int fd, void *buf, size_t len)
{
	while (len > 0) {
		ssize_t l = read(fd, buf, len);
		if (l == 0)
			return errmsg("eof reached; %zu bytes remaining", len);
		else if (l > 0) {
			buf += l;
			len -= l;
		} else if (errno == EINTR || errno == EAGAIN)
			continue;
		else
			return sys_errmsg("reading failed; %zu bytes remaining", len);
	}

	return 0;
}

/*
 * Returns %-1 if consecutive bad blocks exceeds the
 * MAX_CONSECUTIVE_BAD_BLOCKS and returns %0 otherwise.
 */
static int consecutive_bad_check(int eb)
{
	static int consecutive_bad_blocks = 1;
	static int prev_bb = -1;

	if (prev_bb == -1)
		prev_bb = eb;

	if (eb == prev_bb + 1)
		consecutive_bad_blocks += 1;
	else
		consecutive_bad_blocks = 1;

	prev_bb = eb;

	if (consecutive_bad_blocks >= MAX_CONSECUTIVE_BAD_BLOCKS) {
		if (!args.quiet)
			printf("\n");
		return errmsg("consecutive bad blocks exceed limit: %d, bad flash?",
				MAX_CONSECUTIVE_BAD_BLOCKS);
	}

	return 0;
}

/* TODO: we should actually torture the PEB before marking it as bad */
static int mark_bad(const struct mtd_dev_info *mtd, struct ubi_scan_info *si, int eb)
{
	int err;

	if (!args.yes)
		if (!answer_is_yes("mark it as bad?"))
			return -1;

	if (!args.quiet)
		normsg_cont("marking block %d bad", eb);

	if (!args.quiet)
		printf("\n");

	if (!mtd->bb_allowed) {
		if (!args.quiet)
			printf("\n");
		return errmsg("bad blocks not supported by this flash");
	}

	err = mtd_mark_bad(mtd, args.node_fd, eb);
	if (err)
		return err;

	si->bad_cnt += 1;
	si->ec[eb] = EB_BAD;

	return consecutive_bad_check(eb);
}

static int flash_image(struct img_type *img, libmtd_t libmtd,
		       const struct mtd_dev_info *mtd,
		       const struct ubigen_info *ui, struct ubi_scan_info *si)
{
	int img_ebs, eb, written_ebs = 0, divisor, skip_data_read = 0;
	off_t st_size = img->size;
	int fd = img->fdin;

	img_ebs = st_size / mtd->eb_size;

	if (img_ebs > si->good_cnt) {
		sys_errmsg("file \"%s\" is too large (%lld bytes)",
			   args.image, (long long)st_size);
		goto out_close;
	}

	if (st_size % mtd->eb_size) {
		return sys_errmsg("file \"%s\" (size %lld bytes) is not multiple of ""eraseblock size (%d bytes)",
				  args.image, (long long)st_size, mtd->eb_size);
		goto out_close;
	}

	verbose(args.verbose, "will write %d eraseblocks", img_ebs);
	divisor = img_ebs;
	for (eb = 0; eb < mtd->eb_cnt; eb++) {
		int err, new_len;
		char buf[mtd->eb_size];
		long long ec;

		if (!args.quiet && !args.verbose) {
			printf("\r" PROGRAM_NAME ": flashing eraseblock %d -- %2lld %% complete  ",
			       eb, (long long)(eb + 1) * 100 / divisor);
			fflush(stdout);
		}

		if (si->ec[eb] == EB_BAD) {
			divisor += 1;
			continue;
		}

		if (args.verbose) {
			normsg_cont("eraseblock %d: erase", eb);
			fflush(stdout);
		}

		err = mtd_erase(libmtd, mtd, args.node_fd, eb);
		if (err) {
			if (!args.quiet)
				printf("\n");
			sys_errmsg("failed to erase eraseblock %d", eb);

			if (errno != EIO)
				goto out_close;

			if (mark_bad(mtd, si, eb))
				goto out_close;

			continue;
		}

		if (!skip_data_read) {
			err = read_all(fd, buf, mtd->eb_size);
			if (err) {
				sys_errmsg("failed to read eraseblock %d from \"%s\"",
					   written_ebs, args.image);
				goto out_close;
			}
		}
		skip_data_read = 0;

		if (args.override_ec)
			ec = args.ec;
		else if (si->ec[eb] <= EC_MAX)
			ec = si->ec[eb] + 1;
		else
			ec = si->mean_ec;

		if (args.verbose) {
			printf(", change EC to %lld", ec);
			fflush(stdout);
		}

		err = change_ech((struct ubi_ec_hdr *)buf, ui->image_seq, ec);
		if (err) {
			errmsg("bad EC header at eraseblock %d of \"%s\"",
			       written_ebs, args.image);
			goto out_close;
		}

		if (args.verbose) {
			printf(", write data\n");
			fflush(stdout);
		}

		new_len = drop_ffs(mtd, buf, mtd->eb_size);

		err = mtd_write(libmtd, mtd, args.node_fd, eb, 0, buf, new_len,
				NULL, 0, 0);
		if (err) {
			sys_errmsg("cannot write eraseblock %d", eb);

			if (errno != EIO)
				goto out_close;

			err = mtd_torture(libmtd, mtd, args.node_fd, eb);
			if (err) {
				if (mark_bad(mtd, si, eb))
					goto out_close;
			}

			/*
			 * We have to make sure that we do not read next block
			 * of data from the input image or stdin - we have to
			 * write buf first instead.
			 */
			skip_data_read = 1;
			continue;
		}
		if (++written_ebs >= img_ebs)
			break;
	}

	if (!args.quiet && !args.verbose)
		printf("\n");

	return eb + 1;

out_close:
	return -1;
}

static int format(libmtd_t libmtd, const struct mtd_dev_info *mtd,
		  const struct ubigen_info *ui, struct ubi_scan_info *si,
		  int start_eb, int novtbl)
{
	int eb, err, write_size;
	struct ubi_ec_hdr *hdr;
	struct ubi_vtbl_record *vtbl;
	int eb1 = -1, eb2 = -1;
	long long ec1 = -1, ec2 = -1;

	write_size = UBI_EC_HDR_SIZE + mtd->subpage_size - 1;
	write_size /= mtd->subpage_size;
	write_size *= mtd->subpage_size;
	hdr = malloc(write_size);
	if (!hdr)
		return sys_errmsg("cannot allocate %d bytes of memory", write_size);
	memset(hdr, 0xFF, write_size);

	for (eb = start_eb; eb < mtd->eb_cnt; eb++) {
		long long ec;

		if (!args.quiet && !args.verbose) {
			printf("\r" PROGRAM_NAME ": formatting eraseblock %d -- %2lld %% complete  ",
			       eb, (long long)(eb + 1 - start_eb) * 100 / (mtd->eb_cnt - start_eb));
			fflush(stdout);
		}

		if (si->ec[eb] == EB_BAD)
			continue;

		if (args.override_ec)
			ec = args.ec;
		else if (si->ec[eb] <= EC_MAX)
			ec = si->ec[eb] + 1;
		else
			ec = si->mean_ec;
		ubigen_init_ec_hdr(ui, hdr, ec);

		if (args.verbose) {
			normsg_cont("eraseblock %d: erase", eb);
			fflush(stdout);
		}

		err = mtd_erase(libmtd, mtd, args.node_fd, eb);
		if (err) {
			if (!args.quiet)
				printf("\n");

			sys_errmsg("failed to erase eraseblock %d", eb);
			if (errno != EIO)
				goto out_free;

			if (mark_bad(mtd, si, eb))
				goto out_free;
			continue;
		}

		if ((eb1 == -1 || eb2 == -1) && !novtbl) {
			if (eb1 == -1) {
				eb1 = eb;
				ec1 = ec;
			} else if (eb2 == -1) {
				eb2 = eb;
				ec2 = ec;
			}
			if (args.verbose)
				printf(", do not write EC, leave for vtbl\n");
			continue;
		}

		if (args.verbose) {
			printf(", write EC %lld\n", ec);
			fflush(stdout);
		}

		err = mtd_write(libmtd, mtd, args.node_fd, eb, 0, hdr,
				write_size, NULL, 0, 0);
		if (err) {
			if (!args.quiet && !args.verbose)
				printf("\n");
			sys_errmsg("cannot write EC header (%d bytes buffer) to eraseblock %d",
				   write_size, eb);

			if (errno != EIO) {
				if (args.subpage_size != mtd->min_io_size)
					normsg("may be sub-page size is incorrect?");
				goto out_free;
			}

			err = mtd_torture(libmtd, mtd, args.node_fd, eb);
			if (err) {
				if (mark_bad(mtd, si, eb))
					goto out_free;
			}
			continue;

		}
	}

	if (!args.quiet && !args.verbose)
		printf("\n");

	if (!novtbl) {
		if (eb1 == -1 || eb2 == -1) {
			errmsg("no eraseblocks for volume table");
			goto out_free;
		}

		verbose(args.verbose, "write volume table to eraseblocks %d and %d", eb1, eb2);
		vtbl = ubigen_create_empty_vtbl(ui);
		if (!vtbl)
			goto out_free;

		err = ubigen_write_layout_vol(ui, eb1, eb2, ec1,  ec2, vtbl,
					      args.node_fd);
		free(vtbl);
		if (err) {
			errmsg("cannot write layout volume");
			goto out_free;
		}
	}

	free(hdr);
	return 0;

out_free:
	free(hdr);
	return -1;
}

static int update_ubi_image(struct img_type *img)
{
	int err, verbose;
	libmtd_t libmtd;
	struct mtd_info mtd_info;
	struct mtd_dev_info mtd;
#ifdef UBI_CHECK_ATTACHED
	libubi_t libubi;
#endif
	struct ubigen_info ui;
	struct ubi_scan_info *si;

	libmtd = libmtd_open();
	if (!libmtd)
		return errmsg("MTD subsystem is not present");

	err = parse_opt(img);
	if (err)
		goto out_close_mtd;

	err = mtd_get_info(libmtd, &mtd_info);
	if (err) {
		sys_errmsg("cannot get MTD information");
		goto out_close_mtd;
	}

	err = mtd_get_dev_info(libmtd, args.node, &mtd);
	if (err) {
		sys_errmsg("cannot get information about \"%s\"", args.node);
		goto out_close_mtd;
	}

	if (!is_power_of_2(mtd.min_io_size)) {
		errmsg("min. I/O size is %d, but should be power of 2",
		       mtd.min_io_size);
		goto out_close_mtd;
	}

	if (!mtd_info.sysfs_supported) {
		/*
		 * Linux kernels older than 2.6.30 did not support sysfs
		 * interface, and it is impossible to find out sub-page
		 * size in these kernels. This is why users should
		 * provide -s option.
		 */
		if (args.subpage_size == 0) {
			warnmsg("your MTD system is old and it is impossible "
				"to detect sub-page size. Use -s to get rid "
				"of this warning");
			normsg("assume sub-page to be %d", mtd.subpage_size);
		} else {
			mtd.subpage_size = args.subpage_size;
			args.manual_subpage = 1;
		}
	} else if (args.subpage_size && args.subpage_size != mtd.subpage_size) {
		mtd.subpage_size = args.subpage_size;
		args.manual_subpage = 1;
	}

	if (args.manual_subpage) {
		/* Do some sanity check */
		if (args.subpage_size > mtd.min_io_size) {
			errmsg("sub-page cannot be larger than min. I/O unit");
			goto out_close_mtd;
		}

		if (mtd.min_io_size % args.subpage_size) {
			errmsg("min. I/O unit size should be multiple of "
			       "sub-page size");
			goto out_close_mtd;
		}
	}

	args.node_fd = open(args.node, O_RDWR);
	if (args.node_fd == -1) {
		sys_errmsg("cannot open \"%s\"", args.node);
		goto out_close_mtd;
	}

	/* Validate VID header offset if it was specified */
	if (args.vid_hdr_offs != 0) {
		if (args.vid_hdr_offs % 8) {
			errmsg("VID header offset has to be multiple of min. I/O unit size");
			goto out_close;
		}
		if (args.vid_hdr_offs + (int)UBI_VID_HDR_SIZE > mtd.eb_size) {
			errmsg("bad VID header offset");
			goto out_close;
		}
	}

	if (!mtd.writable) {
		errmsg("mtd%d (%s) is a read-only device", mtd.mtd_num, args.node);
		goto out_close;
	}

#ifdef UBI_CHECK_ATTACHED
	/* Make sure this MTD device is not attached to UBI */
	libubi = libubi_open();
	if (libubi) {
		int ubi_dev_num;

		err = mtd_num2ubi_dev(libubi, mtd.mtd_num, &ubi_dev_num);
		libubi_close(libubi);
		if (!err) {
			errmsg("please, first detach mtd%d (%s) from ubi%d",
			       mtd.mtd_num, args.node, ubi_dev_num);
			goto out_close;
		}
	}
#endif

	if (!args.quiet) {
		normsg_cont("mtd%d (%s), size ", mtd.mtd_num, mtd.type_str);
		util_print_bytes(mtd.size, 1);
		printf(", %d eraseblocks of ", mtd.eb_cnt);
		util_print_bytes(mtd.eb_size, 1);
		printf(", min. I/O size %d bytes\n", mtd.min_io_size);
	}

	if (args.quiet)
		verbose = 0;
	else if (args.verbose)
		verbose = 2;
	else
		verbose = 1;
	err = ubi_scan(&mtd, args.node_fd, &si, verbose);
	if (err) {
		errmsg("failed to scan mtd%d (%s)", mtd.mtd_num, args.node);
		goto out_close;
	}

	if (si->good_cnt == 0) {
		errmsg("all %d eraseblocks are bad", si->bad_cnt);
		goto out_free;
	}

	if (si->good_cnt < 2 && (!args.novtbl || args.image)) {
		errmsg("too few non-bad eraseblocks (%d) on mtd%d",
		       si->good_cnt, mtd.mtd_num);
		goto out_free;
	}

	if (!args.quiet) {
		if (si->ok_cnt)
			normsg("%d eraseblocks have valid erase counter, mean value is %lld",
			       si->ok_cnt, si->mean_ec);
		if (si->empty_cnt)
			normsg("%d eraseblocks are supposedly empty", si->empty_cnt);
		if (si->corrupted_cnt)
			normsg("%d corrupted erase counters", si->corrupted_cnt);
		print_bad_eraseblocks(&mtd, si);
	}

	if (si->alien_cnt) {
		if (!args.yes || !args.quiet)
			warnmsg("%d of %d eraseblocks contain non-UBI data",
				si->alien_cnt, si->good_cnt);
		if (!args.yes && want_exit()) {
			if (args.yes && !args.quiet)
				printf("yes\n");
			goto out_free;
		}
	}

	if (!args.override_ec && si->empty_cnt < si->good_cnt) {
		int percent = (si->ok_cnt * 100) / si->good_cnt;

		/*
		 * Make sure the majority of eraseblocks have valid
		 * erase counters.
		 */
		if (percent < 50) {
			if (!args.yes || !args.quiet) {
				warnmsg("only %d of %d eraseblocks have valid erase counter",
					si->ok_cnt, si->good_cnt);
				normsg("erase counter 0 will be used for all eraseblocks");
				normsg("note, arbitrary erase counter value may be specified using -e option");
			}
			if (!args.yes && want_exit()) {
				if (args.yes && !args.quiet)
					printf("yes\n");
				goto out_free;
			}
			 args.ec = 0;
			 args.override_ec = 1;
		} else if (percent < 95) {
			if (!args.yes || !args.quiet) {
				warnmsg("only %d of %d eraseblocks have valid erase counter",
					si->ok_cnt, si->good_cnt);
				normsg("mean erase counter %lld will be used for the rest of eraseblock",
				       si->mean_ec);
			}
			if (!args.yes && want_exit()) {
				if (args.yes && !args.quiet)
					printf("yes\n");
				goto out_free;
			}
			args.ec = si->mean_ec;
			args.override_ec = 1;
		}
	}

	if (!args.quiet && args.override_ec)
		normsg("use erase counter %lld for all eraseblocks", args.ec);

	ubigen_info_init(&ui, mtd.eb_size, mtd.min_io_size, mtd.subpage_size,
			 args.vid_hdr_offs, args.ubi_ver, args.image_seq);

	if (si->vid_hdr_offs != -1 && ui.vid_hdr_offs != si->vid_hdr_offs) {
		/*
		 * Hmm, what we read from flash and what we calculated using
		 * min. I/O unit size and sub-page size differs.
		 */
		if (!args.yes || !args.quiet) {
			warnmsg("VID header and data offsets on flash are %d and %d, "
				"which is different to requested offsets %d and %d",
				si->vid_hdr_offs, si->data_offs, ui.vid_hdr_offs,
				ui.data_offs);
			normsg_cont("use new offsets %d and %d? ",
				    ui.vid_hdr_offs, ui.data_offs);
		}
		if (args.yes || answer_is_yes(NULL)) {
			if (args.yes && !args.quiet)
				printf("yes\n");
		} else
			ubigen_info_init(&ui, mtd.eb_size, mtd.min_io_size, 0,
					 si->vid_hdr_offs, args.ubi_ver,
					 args.image_seq);
		normsg("use offsets %d and %d",  ui.vid_hdr_offs, ui.data_offs);
	}

	err = flash_image(img, libmtd, &mtd, &ui, si);
	if (err < 0)
		goto out_free;

	err = format(libmtd, &mtd, &ui, si, err, 1);
	if (err)
		goto out_free;

	ubi_scan_free(si);
	close(args.node_fd);
	libmtd_close(libmtd);
	return 0;

out_free:
	ubi_scan_free(si);
out_close:
	close(args.node_fd);
out_close_mtd:
	libmtd_close(libmtd);
	return -1;
}

static int install_ubi_image(struct img_type *img,
			     void __attribute__ ((__unused__)) *data)
{
	return update_ubi_image(img);
}

__attribute__((constructor))
void ubi_image_handler(void)
{
	register_handler("ubi-img", install_ubi_image, IMAGE_HANDLER, NULL);
}
