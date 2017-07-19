# Base this image on core-image-sato
include recipes-sato/images/core-image-sato.bb
LICENSE = "GPLv2"

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
"