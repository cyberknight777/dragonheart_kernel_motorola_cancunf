#
# arch/arm64/boot/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 2012, ARM Ltd.
# Author: Will Deacon <will.deacon@arm.com>
#
# Based on the ia64 boot/Makefile.
#

OBJCOPYFLAGS_Image :=-O binary -R .note -R .note.gnu.build-id -R .comment -S

targets := Image Image.bz2 Image.gz Image.lz4 Image.lzma Image.lzo dtbo.img

DTBO_OBJS := \
$(obj)/dts/mediatek/mt6855-cancunf-evb-overlay.dtbo \
$(obj)/dts/mediatek/mt6855-cancunf-evb-india-overlay.dtbo \
$(obj)/dts/mediatek/mt6855-cancunf-evb-japan-overlay.dtbo \
$(obj)/dts/mediatek/mt6855-cancunf-evb-europe-overlay.dtbo \
$(obj)/dts/mediatek/mt6855-cancunf-evb-india2-overlay.dtbo \
$(obj)/dts/mediatek/mt6855-cancunf-evb-japan2-overlay.dtbo \
$(obj)/dts/mediatek/mt6855-cancunf-evb-europe2-overlay.dtbo \
$(obj)/dts/mediatek/mt6855-cancunf-evb-india3-overlay.dtbo \
$(obj)/dts/mediatek/mt6855-cancunf-evb-jpsb-overlay.dtbo \
$(obj)/dts/mediatek/mt6855-cancunf-evb-prc3-overlay.dtbo \
$(obj)/dts/mediatek/mt6855-cancunf-evb-jpret-overlay.dtbo

$(obj)/Image: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/Image.bz2: $(obj)/Image FORCE
	$(call if_changed,bzip2)

$(obj)/Image.gz: $(obj)/Image FORCE
	$(call if_changed,gzip)

$(obj)/Image.lz4: $(obj)/Image FORCE
	$(call if_changed,lz4)

$(obj)/Image.lzma: $(obj)/Image FORCE
	$(call if_changed,lzma)

$(obj)/Image.lzo: $(obj)/Image FORCE
	$(call if_changed,lzo)

$(obj)/dtbo.img: $(DTBO_OBJS) FORCE
	$(call if_changed,mkdtboimg)

install:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/Image System.map "$(INSTALL_PATH)"

zinstall:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/Image.gz System.map "$(INSTALL_PATH)"
