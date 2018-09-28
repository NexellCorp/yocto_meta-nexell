inherit post-process-qt
inherit post-process
require recipes-graphics/images/core-image-weston.bb

DESCRIPTION ?= "Nexell qt images"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

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

ETC_TOOLS = " \
    tinyalsa \
    udev-extraconf \
    tzdata \
    net-tools \
    openssl \
    curl \
"

#NX_QT_LAUNCHER = "qtsmarthome qt5-smarthome-launcher"
NX_QT_LAUNCHER = "qtcinema-launcher"

NEXELL_CUSTOMIZE_INSTALL = " \
    ${NX_QT_LAUNCHER} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-daudio-ref', 'nexell-bootanim nexell-btservice', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-daudio-covi', 'nexell-bootanim', '', d)} \
    nexell-qt5-touchsetup \
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
    ${DEVEL_TOOLS} \
    packagegroup-nexell-qt \
    weston-init \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    testsuite \
    ${TOUCH_IMAGE_INSTALL} \
    ${ETC_TOOLS} \
    ${NEXELL_CUSTOMIZE_INSTALL} \
"

# pyro -> sumo
# gcc 6.3.0 -> 7.3.0 update: compile error 
#${@bb.utils.contains('NEXELL_BOARD_SOCNAME', 's5p4418', 'NxAudioPlayer NxQuickRearCam NxVideoPlayer', '', d)}

