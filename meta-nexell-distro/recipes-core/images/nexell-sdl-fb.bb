DESCRIPTION = "Extend core-image by adding sdl"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

inherit core-image
inherit post-process
inherit post-process-convergence-svm
require recipes-core/images/core-image-minimal.bb

DEPENDS += " libsdl2"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

DEVEL_TOOLS = " \
    android-tools-nexell \
"

IMAGE_INSTALL_append = " \
    libsdl2 \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
    ${DEVEL_TOOLS} \
    packagegroup-nexell-sdl \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    testsuite \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-convergence-svmc', 'convergence-svmc-init', '', d)} \
"
