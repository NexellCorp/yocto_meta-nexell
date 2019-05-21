#@TYPE: Machine
#@NAME: nxp3220

NEXELL_BOARD_SOCNAME = "nxp3220"

TOOL_BINGEN ?= "${BSP_ROOT_DIR}/tools/bin/bingen"
TOOL_BINENC ?= "${BSP_ROOT_DIR}/tools/bin/aescbc_enc"
TOOL_BINECC ?= "${BSP_ROOT_DIR}/tools/bin/nandbingen"

# for u-boot
# set BINARY_FEATURES_pn-u-boot-2018.5-nexell = "nand" to build ecc image
UBOOT_BIN ?= "u-boot.bin"
UBOOT_NSIH ?= "${BSP_ROOT_DIR}/tools/files/nsih_uboot.txt"
UBOOT_BOOTKEY ?= "${BSP_ROOT_DIR}/tools/files/bootkey"
UBOOT_USERKEY ?= "${BSP_ROOT_DIR}/tools/files/userkey"
UBOOT_LOADADDR ?= "0x43c00000"
UBOOT_LOGO_BMP ?= "${BSP_ROOT_DIR}/tools/files/logo.bmp"

# for BL1
# set BINARY_FEATURES_pn-bl1-nxp3220 = "nand" to build ecc image
BL1_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/binary"
BL1_BIN ?= "bl1-nxp3220.bin"
BL1_LOADADDR ?= "0xFFFF0000"
BL1_NSIH ?= "${BSP_ROOT_DIR}/tools/files/nsih_bl1.txt"
BL1_BOOTKEY ?= "${BSP_ROOT_DIR}/tools/files/bootkey"
BL1_USERKEY ?= "${BSP_ROOT_DIR}/tools/files/userkey"
BL1_AESKEY ?= "${BSP_ROOT_DIR}/tools/files/aeskey.txt"
BL1_VECTOR ?= "${BSP_ROOT_DIR}/tools/files/aesvector.txt"

# for BL2
# Note: BL2_LOADADDR is fixed
# set BINARY_FEATURES_pn-bl2-nxp3220 = "nand" to build ecc image
BL2_CHIP ?= "nxp3220"
BL2_PMIC ?= "nxe1500"
BL2_BIN ?= "bl2-${BL2_BOARD}.bin"
BL2_LOADADDR ?= "0xFFFF9000"
BL2_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/bl2-nxp3220"
BL2_NSIH ?= "${BL2_SOURCE}/reference-nsih/nsih_evb_ddr3_800Mhz.txt"
BL2_BOOTKEY ?= "${BSP_ROOT_DIR}/tools/files/bootkey"
BL2_USERKEY ?= "${BSP_ROOT_DIR}/tools/files/userkey"

# for BL32
# set BINARY_FEATURES_pn-bl32-nxp3220 = "nand" to build ecc image
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

# for NAND parameter
NAND_PAGE_SIZE ?= ""
NAND_BLOCK_SIZE ?= ""
NAND_FLASH_SIZE ?= ""

# for nand ubi images
# IMAGE_CLASSES += " nexell-image-types"
# IMAGE_FSTYPES += " multiubi2"
# IMAGE_FSTYPES_DEBUGFS += " multiubi2"
# UBI_VOLNAME = "flash"
# KUBIFS_ARGS_images options : Each partition is separated by ','
#  -r : input root filesystem for ubi image
#  -v : ubi volume name
#  -i : ubi volume id
#  -l : ubi volume size

# for bluethooth
BSA_SERVICE_COMMAND ?= "/usr/bin/bsa_server -d /dev/ttyS0 -p /etc/bluetooth/BCM434545.hcd -all=0"
