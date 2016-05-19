#!/bin/bash

PARENT_DIR="${PWD%/*}"
META_NEXELL_TOOLS_DIR="${PARENT_DIR}/meta-nexell/tools"

sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap -n ${META_NEXELL_TOOLS_DIR}/raptor-64.txt -b bl1-raptor.bin
sleep 1
sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap -n ${META_NEXELL_TOOLS_DIR}/raptor-64.txt -f u-boot.bin -a 0x43c00000 -j 0x43c00000
