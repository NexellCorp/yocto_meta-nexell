DESCRIPTION = "Nexell SDL SDKs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

inherit core-image
require recipes-core/images/core-image-minimal.bb

IMAGE_INSTALL_append = " \
    kernel-modules \
    packagegroup-nexell-common-sdk \
    packagegroup-nexell-sdl-sdk \
    rtl-8188eus-${ARCH_TYPE_NUM} \
"
