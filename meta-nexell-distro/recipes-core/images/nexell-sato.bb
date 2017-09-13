inherit post-process
include recipes-sato/images/core-image-sato.bb

DESCRIPTION ?= "Nexell sato images"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

#X11 touch calibration
#==> /usr/bin# DISPLAY=:0.0 xinput_calibrator
TOUCH_IMAGE_INSTALL = " \
     tslib \
     tslib-conf \
     tslib-tests \
     tslib-calibrate \
     tslib-nexell \
"

DEVEL_TOOLS = " \
    android-tools-nexell \
"

IMAGE_INSTALL_append = " \
    ${TOUCH_IMAGE_INSTALL} \
    ${DEVEL_TOOLS} \
    packagegroup-nexell-sato \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    testsuite-${NEXELL_BOARD_SOCNAME} \
"

