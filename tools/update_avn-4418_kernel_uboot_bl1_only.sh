#!/bin/bash

PARENT_DIR="${PWD%/*}"
META_NEXELL_TOOLS_DIR="${PARENT_DIR}/meta-nexell/tools/avn-4418"

PARTMAP="${META_NEXELL_TOOLS_DIR}/partmap_emmc.txt"

sudo fastboot flash partmap ${PARTMAP}
sudo fastboot flash 2ndboot bl1-emmcboot.bin
sudo fastboot flash bootloader singleimage-emmcboot.bin
sudo fastboot flash boot boot.img
sudo fastboot flash env ${META_NEXELL_TOOLS_DIR}/params.bin
