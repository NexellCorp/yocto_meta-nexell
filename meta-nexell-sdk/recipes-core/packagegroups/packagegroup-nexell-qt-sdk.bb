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

QT5_ESSENTIAL_IMAGES = " \
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
#qtwebengine-qmlplugins
#qtdeclarative-plugins

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
    ${GSTREAMER10} \
    ${QT_WAYLAND} \
    ${QT5_ESSENTIAL_IMAGES} \
    ${QT_ALSA_IMAGE_INSTALL} \
    ${QT_ALSA_EXTRA_IMAGE_INSTALL} \
"
