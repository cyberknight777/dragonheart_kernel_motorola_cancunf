# MTK include path
subdir-ccflags-y += -I$(srctree)/sound/soc/mediatek/audio_dsp
subdir-ccflags-y += -I$(srctree)/sound/soc/mediatek/common
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/audio_ipi/include
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include

# debugging support (also enables trace_printk)
ccflags-y += -I$(srctree)/sound/soc/codecs/tfa98xx/inc
ccflags-y += -DDEBUG -DTFA_NON_DSP_SOLUTION
ccflags-y += -Werror
ccflags-y += $(call cc-option,-Wno-date-time,)

snd-soc-tfa98xx-objs += src/tfa98xx.o
snd-soc-tfa98xx-objs += src/tfa_container.o
snd-soc-tfa98xx-objs += src/tfa_dsp.o
snd-soc-tfa98xx-objs += src/tfa_init.o

ifdef TFA_DEBUG
ccflags-y += -DTFA_DEBUG -DDEBUG
snd-soc-tfa98xx-objs += src/tfa_debug.o
endif

obj-$(CONFIG_SND_SOC_TFA9874) += snd-soc-tfa98xx.o
