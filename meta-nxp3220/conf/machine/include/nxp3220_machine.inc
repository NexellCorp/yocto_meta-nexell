#@TYPE: Machine
#@NAME: nxp3220

NEXELL_BOARD_SOCNAME = "nxp3220"

BSP_BIN_PATH ?= "${BSP_ROOT_DIR}/tools/bin"
BSP_FILES_PATH ?= "${BSP_ROOT_DIR}/tools/files"
BSP_SCRIPTS_PATH ?= "${BSP_ROOT_DIR}/tools/scripts"

# tools
TOOL_BINGEN ?= "${BSP_BIN_PATH}/bingen"
TOOL_BINENC ?= "${BSP_BIN_PATH}/aescbc_enc"
TOOL_BINECC ?= "${BSP_BIN_PATH}/nandbingen"

# for u-boot
# set BINARY_FEATURES_pn-u-boot-2018.5-nexell = "nand.ecc" for ecc image
UBOOT_BIN ?= "u-boot.bin"
UBOOT_NSIH ?= "${BSP_FILES_PATH}/nsih_uboot.txt"
UBOOT_BOOTKEY ?= "${BSP_FILES_PATH}/bootkey"
UBOOT_USERKEY ?= "${BSP_FILES_PATH}/userkey"
UBOOT_LOADADDR ?= "0x43c00000"
UBOOT_LOGO_BMP ?= "${BSP_FILES_PATH}/logo.bmp"

# for BL1
# set BINARY_FEATURES_pn-bl1-nxp3220 = "nand.ecc" for ecc image
BL1_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/binary"
BL1_BIN ?= "bl1-nxp3220.bin"
BL1_LOADADDR ?= "0xFFFF0000"
BL1_NSIH ?= "${BSP_FILES_PATH}/nsih_bl1.txt"
BL1_BOOTKEY ?= "${BSP_FILES_PATH}/bootkey"
BL1_USERKEY ?= "${BSP_FILES_PATH}/userkey"
BL1_AESKEY ?= "${BSP_FILES_PATH}/aeskey.txt"
BL1_VECTOR ?= "${BSP_FILES_PATH}/aesvector.txt"

# for BL2
# Note: BL2_LOADADDR is fixed
# set BINARY_FEATURES_pn-bl2-nxp3220 = "nand.ecc" for ecc image
BL2_CHIP ?= "nxp3220"
BL2_PMIC ?= "nxe1500"
BL2_BIN ?= "bl2-${BL2_BOARD}.bin"
BL2_LOADADDR ?= "0xFFFF9000"
BL2_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/bl2-nxp3220"
BL2_NSIH ?= "${BL2_SOURCE}/reference-nsih/nsih_evb_ddr3_800Mhz.txt"
BL2_BOOTKEY ?= "${BSP_FILES_PATH}/bootkey"
BL2_USERKEY ?= "${BSP_FILES_PATH}/userkey"

# for BL32
# set BINARY_FEATURES_pn-bl32-nxp3220 = "nand.ecc" for ecc image
BL32_LOADADDR ?= "0x5F000000"
BL32_BIN ?= "bl32.bin"
BL32_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/bl32-nxp3220"
BL32_NSIH ?= "${BL32_SOURCE}/reference-nsih/nsih_general.txt"
BL32_BOOTKEY ?= "${BSP_FILES_PATH}/bootkey"
BL32_USERKEY ?= "${BSP_FILES_PATH}/userkey"
BL32_AESKEY ?= "${BSP_FILES_PATH}/aeskey.txt"
BL32_VECTOR ?= "${BSP_FILES_PATH}/aesvector.txt"
