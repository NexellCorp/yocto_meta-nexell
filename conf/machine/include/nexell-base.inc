SOC_FAMILY = "Nexell"
#include conf/machine/include/soc-family.inc

IMAGE_FSTYPES ?= "tar.bz2 ext4"

KERNEL_IMAGETYPE ?= "Image"

MACHINE_FEATURES += "apm usbhost keyboard vfat ext2 screen mouse touchscreen alsa bluetooth wifi sdio"

MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc"

MACHINE_EXTRA_RRECOMMENDS += "kernel-modules kernel-devicetree"

DEFAULTTUNE_artik710-raptor ?= "aarch64"
DEFAULTTUNE_artik530-raptor ?= "armv7a"
DEFAULTTUNE_avn-4418 ?= "cortexa9-neon"