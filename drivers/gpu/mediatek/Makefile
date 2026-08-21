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

MTK_GPU_VERSION = $(subst ",,$(CONFIG_MTK_GPU_VERSION))

export MTK_PLATFORM = $(subst ",,$(CONFIG_MTK_PLATFORM))

obj-y += gpu_bm/
obj-y += gpueb/

obj-y += ged/
obj-y += hal/
obj-y += gpufreq/

obj-y += gpu_$(word 1,$(MTK_GPU_VERSION))/
