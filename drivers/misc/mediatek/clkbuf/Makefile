MFD_TOP = $(srctree)/drivers/mfd

ccflags-y += -I$(src)/src \
     -I$(MFD_TOP)/

# New GKI
# common clkbuf driver
obj-$(CONFIG_MTK_CLKBUF) += clkbuf.o
ifneq ($(CONFIG_MTK_CLKBUF),)
	clkbuf-y += v1/src/mtk_clkbuf_common.o
	ccflags-y += -I$(srctree)/$(src)/v1/inc
endif

clkbuf-$(CONFIG_MTK_CLKBUF_V1) += v1/src/mtk_clkbuf_ctl.o
clkbuf-$(CONFIG_MTK_CLKBUF_V1) += v1/src/mtk-clkbuf-pmif.o

clkbuf-$(CONFIG_MTK_CLKBUF_V1) += v1/src/mtk-clkbuf-dcxo.o
clkbuf-$(CONFIG_MTK_CLKBUF_V1) += v1/src/mtk-clkbuf-dcxo-6685.o

clkbuf-$(CONFIG_MTK_CLKBUF_V1) += v1/src/mtk-srclken-rc-hw.o
clkbuf-$(CONFIG_MTK_CLKBUF_V1) += v1/src/mtk-srclken-rc-hw-v1.o
