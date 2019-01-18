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
    qtconnectivity \
    qtdeclarative \
    qtdeclarative-plugins \
    qtenginio \
    qtimageformats \
    qtimageformats-plugins \
    qtlocation \
    qtlocation-plugins \
    qtmultimedia \
    qtmultimedia-plugins \
    qtscript \
    qtsensors \
    qtsensors-plugins \
    qtserialport \
    qtsvg \
    qtsvg-plugins \
    qtsystems \
    qtquick1 \
    qtquick1-plugins \
    qtwebkit \
    qtwebsockets \
    qtwebchannel \
    qtxmlpatterns \
"

RDEPENDS_${PN} = " \
    ${PACKAGE_QT5_6_ESSENTIAL}  \
"
