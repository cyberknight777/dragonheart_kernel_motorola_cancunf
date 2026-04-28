#
# Copyright (C) 2016 MediaTek Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See http://www.gnu.org/licenses/gpl-2.0.html for more details.
#

ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/inc/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/platform
ccflags-y += -I$(srctree)/drivers/misc/mediatek/ccmni/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/conn_md/include/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/mt6873
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/scp/include/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/scp/rv/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/memory-amms
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/hif/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/mdpm/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/pbm/
ccflags-y += -DCCCI_KMODULE_ENABLE
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/hif
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/udc/udc_lib_inc

obj-$(CONFIG_MTK_ECCCI_DRIVER) += ccci_md_clk.o
obj-$(CONFIG_MTK_ECCCI_DRIVER) += ccci_rtc.o
obj-$(CONFIG_MTK_ECCCI_DRIVER) += ccci_auxadc.o

obj-$(CONFIG_MTK_ECCCI_DRIVER) += ccci_md_all.o
ccci_md_all-$(CONFIG_MTK_ECCCI_DRIVER) := ccci_core.o ccci_bm.o ccci_modem.o modem_sys1.o \
		udc/udc.o \
		hif/ccci_hif.o

# port start
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/port/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/udc
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/udc/udc_lib_inc
ccflags-y += -I$(srctree)/drivers/misc/mediatek/conn_md/include/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/clkbuf_v1

ccci_md_all-$(CONFIG_MTK_ECCCI_DRIVER) += port/port_cfg.o \
		port/port_char.o \
		port/port_ctlmsg.o \
		port/port_udc.o \
		port/port_ipc.o \
		port/port_net.o \
		port/port_poller.o \
		port/port_proxy.o \
		port/port_rpc.o \
		port/port_smem.o \
		port/port_sysmsg.o \
		port/ccci_swtp.o
# port end
# fsm start
ccflags-y += -I$(srctree)/drivers/misc/mediatek/eccci/fsm/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/scp/mt6873
ccci_md_all-$(CONFIG_MTK_ECCCI_DRIVER) += fsm/ccci_fsm.o \
			fsm/ccci_fsm_ioctl.o \
			fsm/ccci_fsm_monitor.o \
			fsm/ccci_fsm_poller.o \
			fsm/ccci_fsm_ee.o \
			fsm/ccci_fsm_sys.o \
			fsm/mdee_dumper_v1.o \
			fsm/mdee_dumper_v2.o \
			fsm/mdee_dumper_v3.o \
			fsm/mdee_dumper_v5.o
# fsm end

# plat start
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/mt6873
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/pmic/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/emi_bwl/mt6873
ccflags-y += -I$(srctree)/drivers/clk/mediatek
# security objects

ccci_md_all-$(CONFIG_MTK_ECCCI_DRIVER) += platform/ccci_platform.o $(core_obj) $(mach_obj) $(sec_obj) \
		platform/ap_md_reg_dump.o \
		platform/md_sys1_platform.o
# plat end

ccci_md_all-$(CONFIG_MTK_ECCCI_DRIVER) += modem_sys3.o

ccci_fsm_scp-y := fsm/ccci_fsm_scp.o
obj-$(CONFIG_MTK_ECCCI_DRIVER) += ccci_fsm_scp.o

# ccci_ccif.ko
ccci_ccif-y := hif/ccci_ringbuf.o hif/ccci_hif_ccif.o
obj-$(CONFIG_MTK_ECCCI_DRIVER) += ccci_ccif.o

# ccci_dpmaif.ko
ccci_dpmaif-y := hif/ccci_hif_dpmaif.o hif/dpmaif_drv.o \
	hif/ccci_hif_dpmaif_comm.o hif/ccci_hif_dpmaif_v3.o hif/dpmaif_drv_v3.o \
	hif/dpmaif_bat.o hif/dpmaif_bat_v3.o hif/dpmaif_debug.o hif/net_speed.o \
	hif/ccci_hif_dpmaif_v1.o hif/dpmaif_drv_v1.o \
	hif/net_pool.o hif/md_spd_dvfs_fn.o hif/md_spd_dvfs_method.o

obj-$(CONFIG_MTK_ECCCI_DRIVER) += ccci_dpmaif.o
