/*
 * Copyright (c) Artem Bityutskiy, 2007, 2008
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __MTD_UTILS_COMMON_H__
#define __MTD_UTILS_COMMON_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <features.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Verbose messages */
#define verbose(verbose, fmt, ...) do { \
	if (verbose) \
		INFO("%s: " fmt, PROGRAM_NAME, ##__VA_ARGS__); \
} while (0)

/* Normal messages */
#define normsg_cont(fmt, ...) \
	INFO("%s: " fmt, __FILE__, ##__VA_ARGS__)

#define normsg(fmt, ...) \
	INFO("%s: " fmt, __FILE__, ##__VA_ARGS__)

/* Error messages */
#define errmsg(fmt, ...) ({ \
	ERROR("%s: error!: " fmt, __FILE__, ##__VA_ARGS__); \
	-EINVAL; \
})

#define sys_errmsg(fmt, ...) ({ \
	ERROR("%s: error!: " fmt, __FILE__, ##__VA_ARGS__); \
	-EINVAL; \
})

/* Warnings */
#define warnmsg(fmt, ...) \
	WARN("%s: warning!: " fmt, __FILE__, ##__VA_ARGS__)

/* for tagging functions that always exit */
#if defined(__GNUC__) || defined(__clang__)
	#define NORETURN __attribute__((noreturn))
#else
	#define NORETURN
#endif

static inline int is_power_of_2(unsigned long long n)
{
	return (n != 0 && ((n & (n - 1)) == 0));
}

void util_print_bytes(long long bytes, int bracket);
int util_srand(void);

#ifdef __cplusplus
}
#endif

#endif /* !__MTD_UTILS_COMMON_H__ */
