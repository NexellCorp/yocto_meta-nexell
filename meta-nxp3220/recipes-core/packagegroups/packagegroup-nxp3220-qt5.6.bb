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
    qtscript \
    qtquick1 \
    qtquick1-plugins \
    qtquick1-qmlplugins \
    qtbase-examples \
    qtsystems-examples \
    qtscript-examples \
    qtquickcontrols-examples \
    liberation-fonts \
    cinematicexperience \
"

RDEPENDS_${PN} = " \
    ${PACKAGE_QT5_6_ESSENTIAL}  \
"
