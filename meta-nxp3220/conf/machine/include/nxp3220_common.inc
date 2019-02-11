#@TYPE: Machine
#@NAME: nxp3220

NEXELL_BOARD_SOCNAME = "nxp3220"

TOOL_BINGEN ?= "${BSP_ROOT_DIR}/tools/bin/bingen"
TOOL_BINENC ?= "${BSP_ROOT_DIR}/tools/bin/aescbc_enc"

# for u-boot
UBOOT_BIN ?= "u-boot.bin"
UBOOT_NSIH ?= "${BSP_ROOT_DIR}/tools/files/nsih_uboot.txt"
UBOOT_BOOTKEY ?= "${BSP_ROOT_DIR}/tools/files/bootkey"
UBOOT_USERKEY ?= "${BSP_ROOT_DIR}/tools/files/userkey"
UBOOT_LOADADDR ?= "0x43c00000"
UBOOT_LOGO_BMP ?= "${BSP_ROOT_DIR}/tools/files/logo.bmp"

# for BL1
BL1_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/binary"
BL1_BIN ?= "bl1-nxp3220.bin.raw"

# for BL2
BL2_CHIP ?= "nxp3220"
BL2_PMIC ?= "nxe1500"
BL2_BIN ?= "bl2-${BL2_BOARD}.bin"
BL2_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/bl2-nxp3220"
BL2_NSIH ?= "${BL2_SOURCE}/reference-nsih/nsih_evb_ddr3_800Mhz.txt"
BL2_BOOTKEY ?= "${BSP_ROOT_DIR}/tools/files/bootkey"
BL2_USERKEY ?= "${BSP_ROOT_DIR}/tools/files/userkey"

# for bl32
BL32_LOADADDR ?= "0x5F000000"
BL32_BIN ?= "bl32.bin"
BL32_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/bl32-nxp3220"
BL32_NSIH ?= "${BL32_SOURCE}/reference-nsih/nsih_general.txt"
BL32_BOOTKEY ?= "${BSP_ROOT_DIR}/tools/files/bootkey"
BL32_USERKEY ?= "${BSP_ROOT_DIR}/tools/files/userkey"
BL32_AESKEY ?= "${BSP_ROOT_DIR}/tools/files/aeskey.txt"
BL32_VECTOR ?= "${BSP_ROOT_DIR}/tools/files/aesvector.txt"

# for disk partition image
PART_BOOT_LABEL ?= "boot"
PART_BOOT_SIZE ?= "32M"

PART_ROOTFS_LABEL ?= "rootfs"
PART_ROOTFS_SIZE ?= "50M"

PART_DATA_LABEL ?= "data"
PART_DATA_SIZE ?= "200M"

# for bluethooth
BSA_SERVICE_COMMAND ?= "/usr/bin/bsa_server -d /dev/ttyS0 -p /etc/bluetooth/BCM434545.hcd -all=0"

# for touchscreen device
TSDEVICE ?= "/dev/input/event1"
EVDEV_TOUCHSCREEN_PARAMETERS ?= "tslib:/dev/input/event1:invertx:rotate=180"
