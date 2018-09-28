inherit post-process-sdl-wayland
inherit post-process
require recipes-graphics/images/core-image-weston.bb

DESCRIPTION ?= "Nexell sdl images"
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

LIBSDL2_IMAGE_INSTALL = " \
    libsdl2 \
    libsdl2-image \
    libsdl2-mixer \
"

SDL_APPLICATIONS = " \
    sdl-helloworld \
    sdl2-opengles-test \
    sdl-imageview-test \
"

IMAGE_INSTALL_append = " \
    ${SDL_APPLICATIONS} \
    ${LIBSDL2_IMAGE_INSTALL} \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
    ${DEVEL_TOOLS} \
    packagegroup-nexell-sdl \
    weston-init \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    testsuite \
    ${TOUCH_IMAGE_INSTALL} \
    ${ETC_TOOLS} \
"
