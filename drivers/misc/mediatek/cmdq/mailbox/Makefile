# SPDX-License-Identifier: GPL-2.0
#
# Copyright (c) 2021 MediaTek Inc.
#

#==========================================================================
#  Command Queue Common Headers
#==========================================================================

ccflags-y += -I$(srctree)/drivers/iommu \
             -I$(srctree)/drivers/misc/mediatek/iommu/ \
             -I$(srctree)/drivers/misc/mediatek/include \
             -I$(srctree)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)/include \
             -I$(srctree)/drivers/misc/mediatek/mdp/$(MTK_PLATFORM) \
             -I$(srctree)/drivers/misc/mediatek/mmp/ \
             -I$(srctree)/drivers/misc/mediatek/smi/ \
             -I$(srctree)/drivers/misc/mediatek/vcp/rv/ \
             -I$(srctree)/drivers/misc/mediatek/vcp/include/

ccflags-$(CONFIG_DEBUG_FS) += -DCMDQ_MMPROFILE_SUPPORT

#==========================================================================
# Secure Command Queue driver
#==========================================================================

ccflags-y += -DCMDQ_SECURE_SUPPORT

include $(srctree)/drivers/tee/teei/Makefile.include
include $(srctree)/drivers/tee/gud/Makefile.include

ccflags-y += -I$(srctree)/drivers/misc/mediatek/geniezone/public
ccflags-y += -DCMDQ_SECURE_MTEE_SUPPORT

obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-sec-drv.o
cmdq-sec-drv-y += cmdq_sec_mtee.o
cmdq-sec-drv-y += cmdq-sec-mailbox.o
cmdq-sec-drv-y += cmdq-sec-helper.o

#==========================================================================
# Normal Command Queue driver
#==========================================================================

obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += mtk-cmdq-drv-ext.o
mtk-cmdq-drv-ext-y := mtk-cmdq-mailbox-ext.o mtk-cmdq-helper-ext.o cmdq-util.o

obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-test.o

obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6855.o
