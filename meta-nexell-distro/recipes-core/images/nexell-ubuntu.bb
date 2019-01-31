inherit post-process-ubuntu
include recipes-core/images/core-image-base.bb

SPLASH = ""
CORE_IMAGE_BASE_INSTALL = " \
    ${CORE_IMAGE_EXTRA_INSTALL} \
"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

TESTSUITE = "${@bb.utils.contains('DISTRO_FEATURES', 's5p6818-soc-type', '', 'testsuite', d)}"
KSELFTEST = "${@bb.utils.contains('DISTRO_FEATURES', 's5p6818-soc-type', '', 'kselftests', d)}"

IMAGE_INSTALL_append = " \
    packagegroup-nexell-ubuntu \
    kernel-modules \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    ubuntu-support \
    ${TESTSUITE} \
    ${KSELFTEST} \
"
