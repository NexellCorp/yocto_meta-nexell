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
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'qtwayland-examples', '', d)} \
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
#qtwebengine-qmlplugins
#qtdeclarative-plugins

QT_ALSA_LIB_IMAGE_INSTALL = " \
    alsa-lib \
"
QT_ALSA_UTILS_IMAGE_INSTALL = " \
    alsa-utils \
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

MULTIMEDIA_IMAGE_INSTALL = " \
    ffmpeg \
    fswebcam \
"

NEXELL_LIBS = " \
    gst-plugins-camera \
    gst-plugins-renderer \
    gst-plugins-scaler \
    gst-plugins-video-dec \
    gst-plugins-video-sink \
    libdrm \
    nx-drm-allocator \
    nx-gst-meta \
    nx-renderer \
    nx-scaler \
    nx-v4l2 \
    nx-video-api \
"

UTILS_INSTALL = " \
    glibc-utils \
    localedef \
    glibc-localedata-i18n \
    glibc-gconv-ibm850 \
    glibc-gconv-cp1252 \
    glibc-gconv-iso8859-1 \
    glibc-gconv-iso8859-15 \
    glibc-gconv-euc-kr \
"

RDEPENDS_${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali', 'nexell-drm-mali-qt', '', d)}  \
    weston-conf \
    ${GSTREAMER10} \
    ${QT_WAYLAND} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_ESSENTIAL_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_EXTRA_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_EXAMPLE_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.8', '${QT5_8_ESSENTIAL_IMAGES}', '', d)}  \
    ${NEXELL_LIBS} \
    ${UTILS_INSTALL} \
    ${QT_ALSA_LIB_IMAGE_INSTALL} \
    ${QT_ALSA_UTILS_IMAGE_INSTALL} \
"
