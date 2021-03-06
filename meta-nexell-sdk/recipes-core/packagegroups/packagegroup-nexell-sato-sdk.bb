#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "SATO requirements"
DESCRIPTION = "The sato set of packages required by NEXELL"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

GSTREAMER10_X11 = " \
    gstreamer1.0-meta-base \
"

X11_INPUT_IMAGE_INSTALL = " \
    xf86-input-tslib \
"
X11_VIDEO_ARMSOC = " \
    xf86-video-armsoc-nexell \
"
X11_ALSA_IMAGE_INSTALL = " \
    alsa-utils \
"
X11_ALSA_EXTRA_IMAGE_INSTALL = " \
    alsa-utils-alsamixer \
    alsa-utils-midi \
    alsa-utils-aplay \
    alsa-utils-amixer \
    alsa-utils-aconnect \
    alsa-utils-iecset \
    alsa-utils-speakertest \
    alsa-utils-aseqnet \
    alsa-utils-aseqdump \
    alsa-utils-alsactl \
"

RDEPENDS_${PN} = " \
    nexell-drm-mali-sato \
    ${GSTREAMER10_X11} \
    ${X11_INPUT_IMAGE_INSTALL} \
    ${X11_VIDEO_ARMSOC} \
    ${X11_ALSA_IMAGE_INSTALL} \
    ${X11_ALSA_EXTRA_IMAGE_INSTALL} \
"
