# SPDX-License-Identifier: GPL-2.0
#
# Copyright (C) 2018 MediaTek Inc.
#

# Modules Feature Options
include $(srctree)/drivers/misc/mediatek/trusted_mem/Makefile.configs

# Force strictly compiler check
ccflags-y += -Werror

#==========================================================================
# Trusted Memory Subsystem Driver
#==========================================================================
# Ensure module is built only when option is enabled
obj-$(CONFIG_MTK_TRUSTED_MEMORY_SUBSYSTEM) = trusted_mem.o
trusted_mem-y += dev_mgr.o
trusted_mem-y += entry.o
trusted_mem-y += region_mgr.o
trusted_mem-y += ssmr_mgr.o
trusted_mem-y += peer_mgr.o
trusted_mem-y += proc.o
trusted_mem-y += ssheap_proc.o
trusted_mem-y += alloc_api.o
trusted_mem-y += ssmr/memory_ssmr.o
trusted_mem-y += ssheap/ssheap_lib.o

#==========================================================================
# TEE Seecure Memory Devices & Trustzone Peer Driver
#==========================================================================
trusted_mem-y += tee_devices.o
ccflags-y += -DTEE_DEVICES_SUPPORT
trusted_mem-y += tee_impl/tee_invoke.o

trusted_mem-y += tee_impl/tee_ops.o

#==========================================================================
# MTEE Multiple Chunk Devices & Geniezone Peer Driver
#==========================================================================
trusted_mem-y += mtee_devices.o
ccflags-y += -DMTEE_DEVICES_SUPPORT
trusted_mem-y += mtee_impl/mtee_ops.o
trusted_mem-y += mtee_impl/mtee_invoke.o
trusted_mem-y += mtee_impl/tmem_carveout_heap.o

#==========================================================================
# Trusted Memory Subsystem Common Headers
#==========================================================================
ccflags-y += -I$(srctree)/drivers/misc/mediatek/trusted_mem/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/trusted_mem/public/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/trusted_mem/ssmr
ccflags-y += -I$(srctree)/drivers/misc/mediatek/trusted_mem/ssheap
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/

#==========================================================================
# Other Modules Include Header Paths
#==========================================================================
ccflags-y += -I$(srctree)/drivers/misc/mediatek/geniezone/public
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/gz/

ccflags-y += -I$(srctree)/drivers/tee/gud/$(CONFIG_TRUSTONIC_TEE_VERSION)/MobiCoreDriver/public
ccflags-y += -I$(srctree)/drivers/tee/gud/$(CONFIG_TRUSTONIC_TEE_VERSION)/MobiCoreDriver/public/GP

ccflags-y += -I$(srctree)/drivers/tee/teei/$(CONFIG_MICROTRUST_TEE_VERSION)/common/include
