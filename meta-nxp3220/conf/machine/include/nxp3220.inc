include conf/machine/include/tune-cortexa7.inc

DEFAULTTUNE = "cortexa7hf-neon"

EXTRA_IMAGEDEPENDS += "u-boot"

MACHINE_EXTRA_RRECOMMENDS = " kernel-modules kernel-devicetree"
LINUX_VERSION ?= "4.14"

# Skip do_kernel_version_sanity_check
KERNEL_VERSION_SANITY_SKIP = "1"

UBOOT_VERSION ?= "2018.5"
UBOOT_SUFFIX = "bin"
IMAGE_BOOT_FILES ?= "u-boot.${UBOOT_SUFFIX}"

PREFERRED_PROVIDER_u-boot ?= "u-boot-${UBOOT_VERSION}-nexell"
PREFERRED_PROVIDER_virtual/bootloader ?= "u-boot-${UBOOT_VERSION}-nexell"
PREFERRED_PROVIDER_virtual/kernel ?= "linux-${LINUX_VERSION}-nexell"

IMAGE_FSTYPES ?= "tar.gz"

MACHINE_FEATURES = "usbgadget usbhost alsa emmc sdio"
