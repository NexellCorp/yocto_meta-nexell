#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb
# add to image:
# IMAGE_INSTALL_append = "packagegroup-<name>"

SUMMARY = "NXP3220 requirements"
DESCRIPTION = "The NXP3220 application packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGE_NEXELL_APPS = " \
    nx-vip-test \
    nx-video-api-test \
    hwio \
    iso7816-test \
"

RDEPENDS_${PN} = " \
    ${PACKAGE_NEXELL_APPS} \
"
