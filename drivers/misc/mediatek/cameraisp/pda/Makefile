#
# Copyright (C) 2021 MediaTek Inc.
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

$(info $(srctree))
ccflags-y += -I$(srctree)/drivers/misc/mediatek/smi/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/mtk-interconnect/
ccflags-y += -I$(srctree)/drivers/iommu
ccflags-y += -I$(srctree)/drivers/misc/mediatek/iommu
ccflags-$(CONFIG_MTK_IOMMU_V2) += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include/

subdir-ccflags-y += -DPDA_TF_DUMP_6s_1

# driver module
obj-y := camera_pda_dummy.o
obj-$(CONFIG_MTK_CAMERA_ISP_PDA_SUPPORT) += pda_drv_mt6855.o

pda_drv_mt6855-objs := isp_6s/camera_pda.o isp_6s/pda_mt6855.o
