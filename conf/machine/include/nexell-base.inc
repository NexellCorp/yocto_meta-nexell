SOC_FAMILY = "Nexell"
#include conf/machine/include/soc-family.inc

IMAGE_FSTYPES ?= "tar.bz2 ext4"
INITRAMFS_IMAGE = "nexell-ramdisk"

KERNEL_IMAGETYPE_s5p6818-artik710-raptor ?= "Image"
KERNEL_IMAGETYPE_s5p6818-avn-ref ?= "Image"
KERNEL_IMAGETYPE_s5p4418-navi-ref ?= "zImage"
KERNEL_IMAGETYPE_s5p4418-avn-ref ?= "zImage"
KERNEL_IMAGETYPE_s5p4418-smart-voice ?= "zImage"
KERNEL_IMAGETYPE_s5p4418-daudio-ref ?= "zImage"
KERNEL_IMAGETYPE_s5p6818-kick-st ?= "Image"

MACHINE_FEATURES += "apm usbhost keyboard vfat ext2 screen mouse touchscreen bluetooth wifi sdio"

MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc"

MACHINE_EXTRA_RRECOMMENDS += "kernel-modules kernel-devicetree"

DEFAULTTUNE_s5p6818-artik710-raptor ?= "aarch64"
DEFAULTTUNE_s5p6818-avn-ref ?= "aarch64"
DEFAULTTUNE_s5p4418-avn-ref ?= "cortexa9hf-neon"
DEFAULTTUNE_s5p4418-navi-ref ?= "cortexa9hf-neon"
DEFAULTTUNE_s5p4418-daudio-ref ?= "cortexa9hf-neon"
DEFAULTTUNE_s5p4418-smart-voice ?= "cortexa9hf-neon"
DEFAULTTUNE_s5p6818-kick-st ?= "aarch64"

DEFAULT_TIMEZONE="Asia/Seoul"