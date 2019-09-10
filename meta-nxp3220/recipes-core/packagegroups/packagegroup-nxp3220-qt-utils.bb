#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb
# add to image:
# IMAGE_INSTALL_append = "packagegroup-<name>"

SUMMARY = "QT util requirements"
DESCRIPTION = "The Qt util packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

QT_UTILS_INSTALL = " \
    liberation-fonts \
    glibc-utils \
    localedef \
    glibc-localedata-i18n \
    glibc-gconv-ibm850 \
    glibc-gconv-cp1252 \
    glibc-gconv-iso8859-1 \
    glibc-gconv-iso8859-15 \
    glibc-gconv-euc-kr \
    lsof \
"

RDEPENDS_${PN} = " \
    ${QT_UTILS_INSTALL} \
"
