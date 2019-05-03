#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb
# add to image:
# IMAGE_INSTALL_append = "packagegroup-<name>"

SUMMARY = "NXP3220 requirements"
DESCRIPTION = "The NXP3220 Libraries packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGE_NEXELL_LIBS = " \
    nx-v4l2 \
    nx-video-api \
    nx-gst-meta \
    gst-plugins-video-dec \
    gst-plugins-video-sink \
"

RDEPENDS_${PN} = " \
    ${PACKAGE_NEXELL_LIBS} \
"