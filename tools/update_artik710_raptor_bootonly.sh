#!/bin/bash

PARENT_DIR="${PWD%/*}"
META_NEXELL_TOOLS_DIR="${PARENT_DIR}/meta-nexell/tools"

PARTMAP="${META_NEXELL_TOOLS_DIR}/partmap_emmc.txt"

sudo fastboot flash partmap ${PARTMAP}
sudo fastboot flash boot boot.img
