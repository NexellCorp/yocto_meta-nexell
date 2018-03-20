#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "QT requirements"
DESCRIPTION = "The qt set of packages required by NEXELL"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

X11_INPUT_IMAGE_INSTALL = " \
    xf86-input-tslib \
"
X11_VIDEO_ARMSOC = " \
    xf86-video-armsoc-nexell \
"
X11_EXTENSION = " \
    xserver-xorg-extension-glx \
"

X11_QT5_4_ESSENTIAL_IMAGES = " \
    qtbase \
    qtbase-plugins \
    qtimageformats-plugins \
    qtgraphicaleffects-qmlplugins \
    qtdeclarative-plugins \
    qtdeclarative-qmlplugins \
    qtsystems-qmlplugins \
    qtlocation-plugins \
    qtmultimedia-plugins \
    qtmultimedia-qmlplugins \
    qtquickcontrols-qmlplugins \
"
X11_QT5_4_EXAMPLE_IMAGES = " \
    qtsmarthome \
    qt5-demo-extrafiles \
    qtconnectivity-examples \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'qtwayland-examples', '', d)} \
    qtdeclarative-examples \
    qtbase-examples \
    qtscript-examples \
    qtquickcontrols-examples \
"
QT5_8_ESSENTIAL_IMAGES = " \
    nothing \
"
QT_ALSA_LIB_IMAGE_INSTALL = " \
    alsa-lib \
"
UTILS_INSTALL = " \
    glibc-utils \
    glibc-localedata-i18n \
    glibc-gconv-ibm850 \
    glibc-gconv-cp1252 \
    glibc-gconv-iso8859-1 \
    glibc-gconv-iso8859-15 \
    glibc-gconv-euc-kr \
    localedef \
"

RDEPENDS_${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali', 'nexell-drm-mali-sato', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${X11_QT5_4_ESSENTIAL_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${X11_QT5_4_EXAMPLE_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.8', '${QT5_8_ESSENTIAL_IMAGES}', '', d)}  \
    ${X11_INPUT_IMAGE_INSTALL} \
    ${X11_VIDEO_ARMSOC} \
    ${X11_EXTENSION} \
"
