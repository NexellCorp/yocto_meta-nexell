DESCRIPTION = "Extend core-image by adding sdl"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

inherit core-image
inherit post-process
require recipes-core/images/core-image-minimal.bb

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

DEVEL_TOOLS = " \
    android-tools-nexell \
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
    ${DEVEL_TOOLS} \
    packagegroup-nexell-sdl \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    testsuite-${NEXELL_BOARD_SOCNAME} \
"
