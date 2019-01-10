#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb
# add :
# IMAGE_FEATURES += "package-management"
# IMAGE_INSTALL_append = "packagegroup-<name>"


SUMMARY = "Tslib requirements"
DESCRIPTION = "The tslib packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGE_TSLIB = " \
     tslib \
     tslib-conf \
     tslib-tests \
     tslib-calibrate \
"

RDEPENDS_${PN} = " \
    ${PACKAGE_TSLIB} \
"
