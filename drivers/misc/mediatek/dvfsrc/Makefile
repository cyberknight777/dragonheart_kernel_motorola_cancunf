ifeq ($(CONFIG_MTK_GCOV_KERNEL),y)
GCOV_PROFILE := y
endif

ccflags-y += \
	-I$(srctree)/drivers/misc/mediatek/include/mt-plat/ \

ccflags-y += -I$(srctree)/drivers/misc/mediatek/sspm/v3/

obj-$(CONFIG_MTK_DVFSRC_HELPER) += mtk-dvfsrc-helper.o
mtk-dvfsrc-helper-objs += dvfsrc-helper.o dvfsrc-debug-v3.o dvfsrc-sysfs.o dvfsrc-debug-v2.o
