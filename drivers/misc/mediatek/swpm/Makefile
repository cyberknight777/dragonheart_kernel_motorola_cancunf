SWPM_BUILD_FOLDER=$(srctree)/drivers/misc/mediatek/swpm

ifeq ($(CONFIG_MTK_TINYSYS_SSPM_V3),y)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sspm/v3
endif

ifeq ($(CONFIG_MTK_QOS_FRAMEWORK),m)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/qos
endif

ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/
ccflags-y += -I$(SWPM_BUILD_FOLDER)/inc/

subdir-ccflags-y += -I$(SWPM_BUILD_FOLDER)/inc/
subdir-ccflags-y += -I$(SWPM_BUILD_FOLDER)/mtk_swpm_fs/

ccflags-y += -DMTK_SWPM_KERNEL_MODULE
subdir-ccflags-y += -DMTK_SWPM_KERNEL_MODULE

#mtk-swpm fs source files
include $(SWPM_BUILD_FOLDER)/mtk_swpm_fs/Makefile
ccflags-y += -I$(SWPM_BUILD_FOLDER)/mtk_swpm_fs/
MTK_SWPM_FS = $(mtk_swpm_fs_objs:%.o=mtk_swpm_fs/%.o)

#mtk-swpm common source files
SWPM_MODULE += swpm_module.o
SWPM_MODULE += swpm_module_ext.o

#Generic build
SWPM_SRC = $(SWPM_MODULE) $(MTK_SWPM_FS)
obj-$(CONFIG_MTK_SWPM_MODULE) += mtk-swpm.o
mtk-swpm-objs := $(SWPM_SRC)

#PERF ARM PMU with CAP_SYS_ADMIN privilege
obj-$(CONFIG_MTK_SWPM_PERF_ARMV8_PMU) += mtk-swpm-perf-arm-pmu.o
mtk-swpm-perf-arm-pmu-objs := swpm_perf_arm_pmu.o

#Generic subdir build
obj-$(CONFIG_MTK_SWPM_MODULE) += modules/
