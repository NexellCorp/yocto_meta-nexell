# Copyright (C) 2014-2018 Stefano Babic <sbabic@denx.de>
#
# SPDX-License-Identifier:     GPL-2.0-or-later
#
# Note: the handlers are inserted automatically
# via a GCC constructor. To work, the linker should
# not drop the handler if it is not called.
# Handler can be called dynamically based
# on the received image type.

ccflags-y += -I$(src)/include

obj-y += backed_block.o sparse.o sparse_err.o output_file.o sparse_crc32.o sparse_read.o

