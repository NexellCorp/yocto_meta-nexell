inherit post-process-qt
inherit post-process
require recipes-graphics/images/core-image-weston.bb

DESCRIPTION ?= "Nexell qt images"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_FEATURES += "package-management ssh-server-dropbear splash"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

DEVEL_TOOLS = " \
    android-tools-nexell \
"

TOUCH_IMAGE_INSTALL = " \
    tslib \
    tslib-conf \
    tslib-tests \
    tslib-calibrate \
    tslib-nexell \
"

NEXELL_CUSTOMIZE_INSTALL = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', 'qt5-smarthome-launcher', 'qtbootlauncher', d)}  \
    nexell-qt5-touchsetup \
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
    ${DEVEL_TOOLS} \
    packagegroup-nexell-qt \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    testsuite-${NEXELL_BOARD_SOCNAME} \
    ${TOUCH_IMAGE_INSTALL} \
    ${NEXELL_CUSTOMIZE_INSTALL} \
"
