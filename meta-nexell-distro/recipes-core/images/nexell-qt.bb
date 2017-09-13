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

PODO_IMAGES_INSTALL = " \
    podonamu \
    connman \
    connman-client \
"

NEXELL_CUSTOMIZE_INSTALL = " \
    qtbootlauncher \ 
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
    ${DEVEL_TOOLS} \
    ${NEXELL_CUSTOMIZE_INSTALL} \
    packagegroup-nexell-qt \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    testsuite-${NEXELL_BOARD_SOCNAME} \
"
#${PODO_IMAGES_INSTALL}