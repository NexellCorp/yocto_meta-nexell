# Base this image on core-image-minimal
include recipes-core/images/core-image-minimal.bb
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_FEATURES += " dev-pkgs tools-sdk tools-debug debug-tweaks ssh-server-openssh"
SDKIMAGE_FEATURES += " dbg-pkgs staticdev-pkgs"

IMAGE_INSTALL_append= " \
    kernel-modules \
    packagegroup-nexell-common-sdk \
"





