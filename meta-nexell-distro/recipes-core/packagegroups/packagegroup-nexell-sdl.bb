#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "SDL requirements"
DESCRIPTION = "The sdl set of packages required by NEXELL"
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
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-fb', 'nexell-drm-mali-sdl-fb', '', d)}  \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali-wayland', 'nexell-drm-mali-sdl-wayland', '', d)}  \
    weston-conf \
"

#     ${GSTREAMER10} \
#     ${NEXELL_LIBS} \
#     ${UTILS_INSTALL} \
# "
