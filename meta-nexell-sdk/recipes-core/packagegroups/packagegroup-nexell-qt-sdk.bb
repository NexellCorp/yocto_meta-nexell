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

QT5_8_ESSENTIAL_IMAGES = " \
    qtbase \
    qtbase-plugins \
    qtdeclarative-qmlplugins \
    qtlocation-plugins \
    qtlocation-qmlplugins \
    qtmultimedia-plugins \
    qtmultimedia-qmlplugins \
    qt3d-qmlplugins \
    qtwebkit-qmlplugins \
    qtquick1-plugins \
    qtquick1-qmlplugins \
    qtquickcontrols-qmlplugins \
    qtquickcontrols2-qmlplugins \
    qtgraphicaleffects-qmlplugins \
    qtvirtualkeyboard-plugins \
    qtvirtualkeyboard-qmlplugins \
    liberation-fonts \
"

QT_ALSA_IMAGE_INSTALL = " \
    alsa-utils \
"
QT_ALSA_EXTRA_IMAGE_INSTALL = " \
    alsa-utils-alsamixer \
    alsa-utils-midi \
    alsa-utils-aplay \
    alsa-utils-amixer \
    alsa-utils-aconnect \
    alsa-utils-iecset \
    alsa-utils-speakertest \
    alsa-utils-aseqnet \
    alsa-utils-aseqdump \
    alsa-utils-alsactl \
"

RDEPENDS_${PN} = " \
    nexell-drm-mali-qt \
    weston-conf \
    ${GSTREAMER10} \
    ${QT_WAYLAND} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_ESSENTIAL_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_EXTRA_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_EXAMPLE_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.8', '${QT5_8_ESSENTIAL_IMAGES}', '', d)}  \
    ${QT_ALSA_IMAGE_INSTALL} \
    ${QT_ALSA_EXTRA_IMAGE_INSTALL} \
"
