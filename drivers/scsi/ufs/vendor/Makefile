# SPDX-License-Identifier: GPL-2.0
# Mediatek UFS extended features makefile

ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/
ccflags-y += -I$(srctree)/drivers/scsi/ufs/
include $(srctree)/drivers/tee/gud/Makefile.include

obj-$(CONFIG_SCSI_UFS_MEDIATEK) += ufs-mediatek-mod.o
ufs-mediatek-mod-y := ../ufs-mediatek.o
ufs-mediatek-mod-$(CONFIG_UFSFEATURE) += ufsfeature.o ufsfeature-mimic.o
ufs-mediatek-mod-$(CONFIG_UFSHID) += ufshid.o
ufs-mediatek-mod-$(CONFIG_SCSI_SKHID) += ufs-manual-gc.o
ufs-mediatek-mod-$(CONFIG_UFSRINGBUF) += ufsringbuf.o
ufs-mediatek-mod-$(CONFIG_FSCRYPT_WRAPED_KEY_MODE_SUPPORT) += ../ufshcd-moto-crypto.o
