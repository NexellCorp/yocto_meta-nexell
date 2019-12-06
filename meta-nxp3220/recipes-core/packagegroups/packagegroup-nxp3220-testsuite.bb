#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb
# add to image:
# IMAGE_INSTALL_append = "packagegroup-<name>"

SUMMARY = "NXP3220 testsuite requirements"
DESCRIPTION = "The NXP3220 testsuite packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGE_TESTSUITE_NEXELL = " \
    nx-vip-test \
    nx-video-api-test \
    hwio \
    disk-test \
    rtc-alarm \
"

PACKAGE_TESTSUITE_TOOLS = " \
    android-tools \
    evtest \
    libdrm-dev \
    alsa-utils \
    tslib tslib-conf tslib-tests tslib-calibrate \
    iperf3 iproute2 can-utils \
    mtd-utils mtd-utils-ubifs \
    lmbench iozone3 \
"

PACKAGE_TESTSUITE = " \
    ${PACKAGE_TESTSUITE_NEXELL} \
    ${PACKAGE_TESTSUITE_TOOLS} \
"

RDEPENDS_${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'testsuite', '${PACKAGE_TESTSUITE}', '', d)} \
"
