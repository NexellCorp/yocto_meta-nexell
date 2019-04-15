#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb
# add to image:
# IMAGE_INSTALL_append = "packagegroup-<name>"

SUMMARY = "DirectFB requirements"
DESCRIPTION = "The DirectFB packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGE_DIRECTFB_ESSENTIAL = " \
    directfb \
    libdrm \
    libdirectfb-nexell \
"

PACKAGE_DIRECTFB_EXAMPLES = " \
    directfb-examples \
"

RDEPENDS_${PN} = " \
    ${PACKAGE_DIRECTFB_ESSENTIAL}  \
"
