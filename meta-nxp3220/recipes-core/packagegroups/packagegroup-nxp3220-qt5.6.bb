#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb
# add to image:
# IMAGE_INSTALL_append = "packagegroup-<name>"

SUMMARY = "QT 5.6 requirements"
DESCRIPTION = "The Qt 5.6 packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGE_QT5_6_ESSENTIAL = " \
    qtbase \
    qtbase-plugins \
    qtlocation \
    qtlocation-plugins \
    qtmultimedia-plugins \
    qtscript \
    qtquick1 \
    qtquick1-plugins \
    liberation-fonts \
"

QT_V4L_UTILS_IMAGE_INSTALL = " \
    v4l-utils \
"

QT_UTILS_INSTALL = " \
    glibc-utils \
    localedef \
    glibc-localedata-i18n \
    glibc-gconv-ibm850 \
    glibc-gconv-cp1252 \
    glibc-gconv-iso8859-1 \
    glibc-gconv-iso8859-15 \
    glibc-gconv-euc-kr \
    libpcre \
    libpcreposix \
"

RDEPENDS_${PN} = " \
    ${PACKAGE_QT5_6_ESSENTIAL}  \
    ${QT_V4L_UTILS_IMAGE_INSTALL} \
    ${QT_UTILS_INSTALL} \
"
