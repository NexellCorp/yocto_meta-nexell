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

RDEPENDS_${PN} = " \
    nexell-drm-mali-sdl \
    libgcc \
    libstdc++ \
    ${TOUCH_IMAGE_INSTALL} \
    ${SDL_IMAGE_INSTALL} \
    ${MULTIMEDIA_IMAGE_INSTALL} \
"