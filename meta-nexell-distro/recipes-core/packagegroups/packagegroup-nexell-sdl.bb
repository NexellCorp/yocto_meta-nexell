#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "SDL requirements"
DESCRIPTION = "The sdl set of packages required by NEXELL"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

TOUCH_IMAGE_INSTALL = " \
    tslib \
    tslib-conf \
    tslib-tests \
    tslib-calibrate \
    tslib-nexell \
"

SDL_IMAGE_INSTALL = " \
    libsdl2 \
"

MULTIMEDIA_IMAGE_INSTALL = " \
    pulseaudio \
    pulseaudio-server \
"
#    ffmpeg
#    fswebcam


NEXELL_LIBS = " \
    libdrm \
    nx-drm-allocator \
    nx-gst-meta \
    nx-renderer \
    nx-scaler \
    nx-v4l2 \
    nx-video-api \
"

RDEPENDS_${PN} = " \
    libgcc \
    libstdc++ \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali', 'nexell-drm-mali-sdl', '', d)}  \
    ${TOUCH_IMAGE_INSTALL} \
    ${SDL_IMAGE_INSTALL} \
    ${MULTIMEDIA_IMAGE_INSTALL} \
    ${NEXELL_LIBS} \
"