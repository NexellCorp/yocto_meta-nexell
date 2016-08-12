#!/bin/bash

PARENT_DIR="${PWD%/*}"
META_NEXELL_TOOLS_DIR="${PARENT_DIR}/meta-nexell/tools/s5p6818-avn-ref"

PARTMAP="${META_NEXELL_TOOLS_DIR}/partmap_emmc.txt"

sudo fastboot flash partmap ${PARTMAP}
sudo fastboot flash 2ndboot bl1-emmcboot.img
sudo fastboot flash fip-loader fip-loader-emmc.img
sudo fastboot flash fip-secure fip-secure.img
sudo fastboot flash fip-nonsecure fip-nonsecure.img
sudo fastboot flash boot boot.img
