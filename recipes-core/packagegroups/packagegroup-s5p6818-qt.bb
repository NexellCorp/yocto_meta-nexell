#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "QT requirements"
DESCRIPTION = "The qt set of packages required by NEXELL AVN"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

WESTON_CONF = " \
    weston-conf \
    "

MX = " \
    mx-1.0 \
    mx-tests \
    "

RDEPENDS_${PN} = " \
    nexell-drm-mali \
    testsuite-s5p6818 \
    rtl-8188eus-64 \
    ${WESTON_CONF} \
    ${MX} \
    "
