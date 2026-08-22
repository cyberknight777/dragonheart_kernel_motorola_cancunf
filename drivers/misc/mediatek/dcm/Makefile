ccflags-y += -I$(srctree)/include/linux/soc/mediatek/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/dcm/include/

# Core
obj-$(CONFIG_MTK_DCM_COMMON)	+= mtk_dcm.o

# SoC Drivers
obj-$(CONFIG_MTK_DCM_MT6855) += mt6855_dcm.o
mt6855_dcm-objs := mt6855_dcm_internal.o mt6855_dcm_autogen.o
