SOC_FAMILY = "Nexell"
#include conf/machine/include/soc-family.inc

IMAGE_FSTYPES ?= "tar.bz2 ext4"

XSERVER = " \
    xserver-xorg \
    xf86-input-evdev \
    xf86-input-mouse \
    xf86-input-keyboard \
    xf86-video-fbdev \
    "

KERNEL_IMAGETYPE ?= "Image"

MACHINE_FEATURES += "apm usbhost keyboard vfat ext2 screen mouse touchscreen alsa bluetooth wifi sdio"

MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc"

MACHINE_EXTRA_RRECOMMENDS += "kernel-modules kernel-devicetree"
