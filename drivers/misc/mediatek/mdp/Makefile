#
# Copyright (C) 2019 MediaTek Inc.
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

#==========================================================================
# MDP Common Headers
#==========================================================================

ccflags-y += -I$(srctree)/drivers/misc/mediatek/smi/ \
             -I$(srctree)/drivers/misc/mediatek/mmdvfs/ \
             -I$(srctree)/drivers/misc/mediatek/mtk-interconnect/ \
             -I$(srctree)/drivers/misc/mediatek/include/ \
             -I$(srctree)/drivers/misc/mediatek/mmp/ \
             -I$(srctree)/drivers/iommu \
             -I$(srctree)/drivers/misc/mediatek/iommu/ \
             -I$(srctree)/drivers/misc/mediatek/cmdq/mailbox/

#==========================================================================
# MDP Secure path for SVP/HDCP/SecCam
#==========================================================================

ccflags-y += -DCMDQ_SECURE_PATH_SUPPORT
ccflags-y += -I$(srctree)/drivers/dma-buf/heaps

#==========================================================================
# MDP driver module
#==========================================================================

obj-$(CONFIG_MTK_MDP) += cmdq_helper_inf.o
obj-$(CONFIG_MTK_MDP) += mdp_drv_mt6855.o

mdp_drv_mt6855-objs := mdp_drv.o mdp_mt6855.o

mdp_drv-objs := mdp_driver.o \
	mdp_ioctl_ex.o \
	mdp_common.o \
	cmdq_record.o \
	cmdq_virtual.o \
	cmdq_device.o \
	mdp_event_common.o \
	cmdq_subsys_common.o \
	cmdq_helper_ext.o \
	cmdq_mmp.o \
	mdp_fence.o
