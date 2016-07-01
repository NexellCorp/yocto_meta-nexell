#!/bin/bash

PARENT_DIR="${PWD%/*}"
META_NEXELL_TOOLS_DIR="${PARENT_DIR}/meta-nexell/tools"

sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap -n ${META_NEXELL_TOOLS_DIR}/avn/nsih_avn_ref_usb.txt -b bl1-avn.bin
sleep 1
sudo ${META_NEXELL_TOOLS_DIR}/usb-downloader -t slsiap -n ${META_NEXELL_TOOLS_DIR}/avn/nsih_avn_ref_usb.txt -f u-boot.bin -a 0x43c00000 -j 0x43c00000
