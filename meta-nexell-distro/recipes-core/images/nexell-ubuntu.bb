inherit post-process-ubuntu
include recipes-core/images/core-image-base.bb

SPLASH = ""
CORE_IMAGE_BASE_INSTALL = " \
    ${CORE_IMAGE_EXTRA_INSTALL} \
"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_LINGUAS = ""

IMAGE_INSTALL_append = " \
    packagegroup-nexell-ubuntu \
    kernel-modules \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    ubuntu-support \
    testsuite \
    kselftests \
"
