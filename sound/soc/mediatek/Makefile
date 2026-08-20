# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_SND_SOC_MEDIATEK) += common/
obj-$(CONFIG_SND_SOC_MT2701) += mt2701/
obj-$(CONFIG_SND_SOC_MT6797) += mt6797/
obj-$(CONFIG_SND_SOC_MT6855) += mt6855/
obj-$(CONFIG_SND_SOC_MT8173) += mt8173/
obj-$(CONFIG_SND_SOC_MT8183) += mt8183/

# vow related code
obj-$(CONFIG_MTK_VOW_SUPPORT) += vow/

# audio sound dsp related code
obj-$(CONFIG_SND_SOC_MTK_AUDIO_DSP) += audio_dsp/

#scp ultra sound related code
obj-$(CONFIG_MTK_ULTRASND_PROXIMITY) += ultrasound/

obj-$(CONFIG_MTK_SCP_AUDIO) += audio_scp/

# include path
subdir-ccflags-y += -Werror -I$(srctree)/drivers/misc/mediatek/include/mt-plat
subdir-ccflags-y += -Werror -I$(srctree)/drivers/misc/mediatek/include
