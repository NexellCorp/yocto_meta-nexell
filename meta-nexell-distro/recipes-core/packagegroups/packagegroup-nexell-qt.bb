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
    qtlocation \
    qtlocation-plugins \
    qtlocation-qmlplugins \
    qtdeclarative-qmlplugins \
    qtmultimedia-plugins \
    qtmultimedia-qmlplugins \
    qt3d-qmlplugins \
    qtgraphicaleffects-qmlplugins \
"

QT5_4_ESSENTIAL_IMAGES = " \
    qtimageformats-plugins \
    qtdeclarative-plugins \
    qtsystems-qmlplugins \
"
QT5_4_EXTRA_IMAGES = " \
    maliit-framework-qt5 \
    maliit-plugins-qt5 \
"
QT5_4_EXAMPLE_IMAGES = " \
    qtsmarthome \
    qt5-demo-extrafiles \
    qtconnectivity-examples \
    qtdeclarative-examples \
    qtbase-examples \
    qtsystems-examples \
    qtmultimedia-examples \
    qtlocation-examples \
    qtscript-examples \
    qtquickcontrols-examples \
    qtquickcontrols-qmlplugins \
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

#qtwebengine-qmlplugins
#qtdeclarative-plugins

QT_ALSA_LIB_IMAGE_INSTALL = " \
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
QT_V4L_UTILS_IMAGE_INSTALL = " \
    v4l-utils \
"

QT_WEBKIT = " \
    qtwebkit \
    qtwebkit-qmlplugins \
    qtsvg \
    qtsvg-plugins \
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
    nexell-drm-mali-qt \
    weston-conf \
    ${GSTREAMER10} \
    ${QT5_ESSENTIAL_IMAGES} \
    ${QT_WAYLAND} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_ESSENTIAL_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_EXTRA_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT5_4_EXAMPLE_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', '${QT_ALSA_LIB_IMAGE_INSTALL}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.6', '${QT5_6_ESSENTIAL_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.8', '${QT5_8_ESSENTIAL_IMAGES}', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.10', '${QT5_10_ESSENTIAL_IMAGES}', '', d)}  \
    ${NEXELL_LIBS} \
    ${UTILS_INSTALL} \
    ${QT_ALSA_UTILS_IMAGE_INSTALL} \
    ${QT_V4L_UTILS_IMAGE_INSTALL} \
"
