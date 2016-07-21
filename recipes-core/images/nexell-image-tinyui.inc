DESCRIPTION = "Extend core-image by adding Qt5"
LICENSE = "GPLv2"

inherit core-image
require recipes-graphics/images/core-image-weston.bb

SPLASH = "psplash-nexell"

IMAGE_FEATURES += "package-management ssh-server-dropbear tools-debug splash"

IMAGE_INSTALL += " \
        kernel-modules \
        "

TOUCH_IMAGE_INSTALL = " \
     tslib \
     tslib-conf \
     tslib-tests \
     tslib-calibrate \
     "

IMAGE_INSTALL += " \
     ${TOUCH_IMAGE_INSTALL} \
     "