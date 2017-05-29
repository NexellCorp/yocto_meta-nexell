#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "SDL UI requirements"
DESCRIPTION = "The sdl set of packages required by NEXELL S5P4418 NAVI"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

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
    nexell-drm-mali-sdl \
    ${NEXELL_LIBS} \
    "