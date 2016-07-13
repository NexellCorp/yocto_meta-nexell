SOC_FAMILY = "Nexell"
#include conf/machine/include/soc-family.inc

IMAGE_FSTYPES ?= "tar.bz2 ext4"

KERNEL_IMAGETYPE ?= "Image"

MACHINE_FEATURES += "apm usbhost keyboard vfat ext2 screen mouse touchscreen alsa bluetooth wifi sdio"

MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc"

MACHINE_EXTRA_RRECOMMENDS += "kernel-modules kernel-devicetree"

DEFAULTTUNE_s5p6818-artik710-raptor ?= "aarch64"
#DEFAULTTUNE_artik530-raptor ?= "armv7a"
DEFAULTTUNE_s5p4418-avn-ref ?= "cortexa9hf-neon"
DEFAULTTUNE_s5p4418-navi-ref ?= "cortexa9hf-neon"
