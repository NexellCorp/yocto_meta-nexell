#@TYPE: Machine
#@NAME: nxp3220

NEXELL_BOARD_SOCNAME = "nxp3220"

BSP_BIN_PATH ?= "${BSP_ROOT_DIR}/tools/bin"
BSP_FILES_PATH ?= "${BSP_ROOT_DIR}/tools/files"
BSP_SCRIPTS_PATH ?= "${BSP_ROOT_DIR}/tools/scripts"

# tools
TOOL_BINGEN ?= "${BSP_BIN_PATH}/bingen"
TOOL_BINECC ?= "${BSP_BIN_PATH}/nandbingen"
TOOL_BINENC = "openssl"

# secure keys
SECURE_BOOTKEY ?= "${BSP_FILES_PATH}/secure-bootkey.pem"
SECURE_USERKEY ?= "${BSP_FILES_PATH}/secure-userkey.pem"
SECURE_BL1_ENCKEY ?= "${BSP_FILES_PATH}/secure-bl1-enckey.txt"
SECURE_BL1_IVECTOR = "${BSP_FILES_PATH}/secure-bl1-ivector.txt"
SECURE_BL32_ENCKEY ?= "${BSP_FILES_PATH}/secure-bl32-enckey.txt"

# for u-boot
# set BINARY_FEATURES_pn-u-boot-2018.5-nexell = "nand.ecc" for ecc image
UBOOT_BIN ?= "u-boot.bin"
UBOOT_NSIH ?= "${BSP_FILES_PATH}/nsih_uboot.txt"
UBOOT_LOADADDR ?= "0x43c00000"
UBOOT_LOGO_BMP ?= "${BSP_FILES_PATH}/logo.bmp"

# for BL1
# set BINARY_FEATURES_pn-bl1-nxp3220 = "nand.ecc" for ecc image
BL1_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/binary"
BL1_BIN ?= "bl1-nxp3220.bin"
BL1_LOADADDR ?= "0xFFFF0000"
BL1_NSIH ?= "${BSP_FILES_PATH}/nsih_bl1.txt"

# for BL2
# Note: BL2_LOADADDR is fixed
# set BINARY_FEATURES_pn-bl2-nxp3220 = "nand.ecc" for ecc image
BL2_CHIP ?= "nxp3220"
BL2_PMIC ?= "nxe1500"
BL2_BIN ?= "bl2-${BL2_BOARD}.bin"
BL2_LOADADDR ?= "0xFFFF9000"
BL2_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/bl2-nxp3220"
BL2_NSIH ?= "${BL2_SOURCE}/reference-nsih/nsih_evb_ddr3_800Mhz.txt"

# for BL32
# set BINARY_FEATURES_pn-bl32-nxp3220 = "nand.ecc" for ecc image
BL32_LOADADDR ?= "0x5F000000"
BL32_BIN ?= "bl32.bin"
BL32_SOURCE ?= "${BSP_ROOT_DIR}/firmwares/bl32-nxp3220"
BL32_NSIH ?= "${BL32_SOURCE}/reference-nsih/nsih_general.txt"
