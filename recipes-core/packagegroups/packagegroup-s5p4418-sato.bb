#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "SATO requirements"
DESCRIPTION = "The sato set of packages required by NEXELL S5p4418"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

GSTREAMER10_X11 = " \
    gstreamer1.0-meta-base \
    "

NEXELL_EXAMPLES = " \
    helloworld-nexell \
    helloworld-nexell-module \
    "

OARDEPENDS_${PN} = " \
    ${GSTREAMER10_X11} \
    nexell-drm-mali \
    testsuite-s5p4418 \
    rtl-8188eus-32 \
    ${NEXELL_EXAMPLES} \
    "