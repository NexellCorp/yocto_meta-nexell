#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb
# add to image:
# IMAGE_INSTALL_append = "packagegroup-<name>"

SUMMARY = "Alsa utils requirements"
DESCRIPTION = "The alsa utils packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGE_ALSA_UTILS = " \
    alsa-utils \
"

RDEPENDS_${PN} = " \
    ${PACKAGE_ALSA_UTILS} \
"
