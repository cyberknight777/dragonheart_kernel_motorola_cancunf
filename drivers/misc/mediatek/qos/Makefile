ccflags-y += -I$(srctree)/drivers/misc/mediatek/sspm/v3

ccflags-y += -I$(srctree)/drivers/misc/mediatek/tinysys_scmi

ccflags-y += -DMTK_SCMI


obj-$(CONFIG_MTK_QOS_FRAMEWORK) += mtk_qos.o
mtk_qos-objs := mtk_qos_common.o  mtk_qos_bound.o  mtk_qos_sysfs.o mtk_qos_share.o qos-v6893.o mtk_qos_scmi.o
