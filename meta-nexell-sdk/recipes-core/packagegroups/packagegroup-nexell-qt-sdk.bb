#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "QT requirements"
DESCRIPTION = "The qt set of packages required by NEXELL"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

GSTREAMER10 = " \
    gstreamer1.0 \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav \
"

QT_WAYLAND = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'qtwayland qtwayland-plugins', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'qtwayland qtwayland-examples', '', d)} \
"

QT5_4_ESSENTIAL_IMAGES = " \
    qtbase \
    qtbase-plugins \
    qtimageformats-plugins \
    qtgraphicaleffects-qmlplugins \
    qtdeclarative-plugins \
    qtdeclarative-qmlplugins \
    qtsystems-qmlplugins \
    qt3d-qmlplugins \
    qtlocation-plugins \
    qtlocation-qmlplugins \
    qtmultimedia-plugins \
    qtmultimedia-qmlplugins \
    qtquickcontrols-qmlplugins \
"
QT5_4_EXTRA_IMAGES = " \
    maliit-framework-qt5 \
    maliit-plugins-qt5 \
"
QT5_4_EXAMPLE_IMAGES = " \
    qtsmarthome \
    qt5-demo-extrafiles \
    qtconnectivity-examples \
    qtwayland-examples \
    qtdeclarative-examples \
    qtbase-examples \
    qtsystems-examples \
    qtmultimedia-examples \
    qtlocation-examples \
    qtscript-examples \
    qtquickcontrols-examples \
    qt3d-examples \
    cinematicexperience \
"

QT5_6_ESSENTIAL_IMAGES = " \
    qtscript \
    qtquick1 \
    qtquick1-plugins \
    qtquick1-qmlplugins \
    liberation-fonts \
    qtbase-examples \
    qtsystems-examples \
    qtmultimedia-examples \
    qtscript-examples \
    qtquickcontrols-examples \
    qt3d-examples \
    cinematicexperience \
"

QT5_10_ESSENTIAL_IMAGES = " \
    qtsmarthome \
    qtquick1-plugins \
    qtquick1-qmlplugins \
    qtquickcontrols2-qmlplugins \
    qtvirtualkeyboard-plugins \
    qtvirtualkeyboard-qmlplugins \
    liberation-fonts \
"

QT_ALSA_LIB_IMAGE_INSTALL = " \
"

QT_ALSA_IMAGE_INSTALL = " \
    alsa-utils \
"

RDEPENDS_${PN} = " \
    nexell-drm-mali-qt \
    weston-conf \
    ${GSTREAMER10} \
    ${QT_WAYLAND} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_ESSENTIAL_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_EXTRA_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_EXAMPLE_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT_ALSA_LIB_IMAGE_INSTALL}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.6', '${QT5_6_ESSENTIAL_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.10', '${QT5_10_ESSENTIAL_IMAGES}', '', d)}  \
    ${QT_ALSA_IMAGE_INSTALL} \
"
