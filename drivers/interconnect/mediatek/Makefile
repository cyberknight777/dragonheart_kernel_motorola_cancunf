# SPDX-License-Identifier: GPL-2.0

ccflags-y += -I$(srctree)/drivers/iommu
ccflags-y += -I$(srctree)/drivers/misc/mediatek/mtk-interconnect
ccflags-y += -I$(srctree)/drivers/misc/mediatek/qos
obj-$(CONFIG_INTERCONNECT_MTK_EMI) += mtk-emi.o
obj-$(CONFIG_INTERCONNECT_MTK_MMQOS_COMMON) += mmqos-common.o
ccflags-y += -I$(srctree)/drivers/misc/mediatek/mmdvfs
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
mmqos-common-objs += mmqos-mtk.o mmqos-hrt.o
obj-$(CONFIG_INTERCONNECT_MTK_MMQOS_MT6855) += mmqos-mt6855.o
