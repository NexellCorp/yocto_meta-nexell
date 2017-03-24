# Base this image on core-image-minimal
include recipes-core/images/core-image-minimal.bb
LICENSE = "GPLv2"

SPLASH ?= "psplash-nexell"

IMAGE_FEATURES += " splash"

SYSV_INSTALL = " \
    udev \
    sysvinit \
    initscripts \
    "

IMAGE_INSTALL += " \
    kernel-modules \
    android-tools \
    ${SYSV_INSTALL} \
    "