#@TYPE: Machine
#@NAME: nxp3220

# for u-boot
UBOOT_NSIH ?= "nsih_uboot.txt"
UBOOT_BOOTKEY ?= "bootkey"
UBOOT_USERKEY ?= "userkey"
UBOOT_LOADADDR ?= "0x43c00000"

# for BL2
BL2_CHIP ?= "nxp3220"
BL2_PMIC ?= "nxe1500"
BL2_BIN ?= "bl2-${BL2_BOARD}.bin"
BL2_NSIH ?= "nsih_evb_ddr3_800Mhz.txt"
BL2_BOOTKEY ?= "bootkey"
BL2_USERKEY ?= "userkey"

# for bl32
BL32_NSIH ?= "nsih_general.txt"
BL32_BOOTKEY ?= "bootkey"
BL32_USERKEY ?= "userkey"
BL32_AESKEY ?= "aeskey.txt"
BL32_VECTOR ?= "aesvector.txt"
BL32_LOADADDR ?= "0x5F000000"

# for disk partition image
PART_BOOT_LABEL ?= "boot"
PART_BOOT_SIZE ?= "32M"

PART_ROOTFS_LABEL ?= "rootfs"
PART_ROOTFS_SIZE ?= "50M"

PART_DATA_LABEL ?= "data"
PART_DATA_SIZE ?= "200M"
