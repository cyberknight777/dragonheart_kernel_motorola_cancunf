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

ifeq ($(strip $(CONFIG_MTK_PQ_COLOR_MODE)),"MDP")
ccflags-y += -DCMDQ_MDP_COLOR
endif

# Enable MET
ccflags-y += -DMET_USER_EVENT_SUPPORT

# Enable AEE
ccflags-y += -DCMDQ_AEE_READY

# iommu callback support
ifeq ($(strip $(CONFIG_MTK_IOMMU)),y)
#ccflags-y += -DMDP_IOMMU_DEBUG
endif

# dump mdp command
#ccflags-y += -DMDP_COMMAND_SIMULATE

#==========================================================================
# MDP Secure path for SVP/HDCP/SecCam
#==========================================================================

ifneq ($(filter y m, $(or $(CONFIG_MTK_TEE_GP_SUPPORT), $(CONFIG_HAVE_MTK_ENABLE_GENIEZONE))),)
ifneq ($(filter y m, $(CONFIG_MTK_MDP_MTEE_SUPPORT)),)
ccflags-y += -DCMDQ_SECURE_PATH_SUPPORT
ccflags-y += -I$(srctree)/drivers/dma-buf/heaps
endif # !CONFIG_MTK_MDP_MTEE_SUPPORT
endif # !SECURE_PATH_SUPPORET

# platform dependent definition
ifneq ($(strip $(CONFIG_MTK_GMO_RAM_OPTIMIZE)),y)
ccflags-y += -DCMDQ_LARGE_MAX_FIRSTERROR_BUFFER
endif # !CONFIG_MTK_GMO_RAM_OPTIMIZE

#==========================================================================
# MDP driver module
#==========================================================================

obj-$(CONFIG_MTK_MDP) += cmdq_helper_inf.o
obj-$(CONFIG_MTK_MDP_MT6873) += mdp_drv_mt6873.o
obj-$(CONFIG_MTK_MDP_MT6853) += mdp_drv_mt6853.o
obj-$(CONFIG_MTK_MDP_MT6761) += mdp_drv_mt6761.o
obj-$(CONFIG_MTK_MDP_MT6739) += mdp_drv_mt6739.o
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

#obj-y += cmdq_prof.o
#obj-y += cmdq_test.o

#ifeq ($(strip $(CONFIG_MMPROFILE)),y)
#obj-y += cmdq_mmp.o
#endif

