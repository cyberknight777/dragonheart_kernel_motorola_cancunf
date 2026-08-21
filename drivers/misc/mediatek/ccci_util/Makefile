#
# Copyright (C) 2015 MediaTek Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#

ccflags-y += -I$(srctree)/drivers/misc/mediatek/ccci_util
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/

obj-m += ccci_util_lib.o
ccci_util_lib-m := ccci_util_lib_fo.o \
			ccci_util_lib_load_img.o \
			ccci_util_lib_sys.o \
			ccci_private_log.o \
			ccci_util_lib_time.o \
			ccci_util_lib_main.o \
			ccci_util_ld_md_errno.o \
			ccci_util_broadcast.o \
			ccci_util_pin_broadcast.o \
			ccci_util_sib.o \
			ccci_util_lib_reserved_mem.o \
			ccci_util_md_rat.o
