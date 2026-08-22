# SPDX-License-Identifier: GPL-2.0

ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/

obj-$(CONFIG_DEVAPC_ARCH_MULTI) += device-apc-common.o
device-apc-common-$(CONFIG_DEVAPC_ARCH_MULTI) += devapc-mtk-multi-ao.o

obj-$(CONFIG_DEVAPC_MT6855) += device-apc-mt6855.o
device-apc-mt6855-$(CONFIG_DEVAPC_MT6855) += devapc-mt6855.o
