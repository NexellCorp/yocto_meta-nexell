DESCRIPTION = "Extend core-image by adding Qt5"
LICENSE = "GPLv2"

inherit core-image
require recipes-graphics/images/core-image-weston.bb

TOUCH_IMAGE_INSTALL = " \
     tslib \
     tslib-conf \
     tslib-tests \
     tslib-calibrate \
     "

IMAGE_INSTALL += " \
     ${TOUCH_IMAGE_INSTALL} \
     can-utils \
     iproute2 \
     "