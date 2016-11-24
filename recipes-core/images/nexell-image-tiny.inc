# Base this image on core-image-minimal
include recipes-core/images/core-image-minimal.bb
LICENSE = "GPLv2"

SPLASH ?= "psplash-nexell"

IMAGE_FEATURES += " splash"

IMAGE_INSTALL += " \
    kernel-modules \
    android-tools \
    "