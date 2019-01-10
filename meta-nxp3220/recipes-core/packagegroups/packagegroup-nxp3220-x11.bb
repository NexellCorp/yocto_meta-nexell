#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "X11 requirements"
DESCRIPTION = "The X11 packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGE_X11_ESSENTIAL = " \
    xdg-utils \
    libxres \
    makedevs \
    mime-support \
    udev-extra-rules \
    connman connman-plugin-loopback connman-plugin-ethernet connman-plugin-wifi connman-client \
    xserver-nodm-init \
    xserver-xorg-extension-dbe \
    xserver-xorg-extension-extmod \
    xauth \
    xhost \
    xset \
    setxkbmap \
    xrdb \
    xorg-minimal-fonts xserver-xorg-utils \
    libxdamage libxvmc libxinerama \
    bash \
    v4l-utils \
    libpcre \
    libpcreposix \
    alsa-states \
"

PACKAGE_X11_EXTENSION = " \
    xserver-xorg-extension-glx \
"

PACKAGE_X11_INPUT = " \
    xf86-input-tslib \
"

PACKAGE_X11_VIDEO_ARMSOC = " \
    xf86-video-armsoc-nexell \
"

RDEPENDS_${PN} = " \
    ${PACKAGE_X11_ESSENTIAL} \
    ${PACKAGE_X11_EXTENSION} \
    ${PACKAGE_X11_INPUT} \
    ${PACKAGE_X11_VIDEO_ARMSOC} \
"
