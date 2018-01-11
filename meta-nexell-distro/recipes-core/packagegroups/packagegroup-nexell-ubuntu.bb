#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "ubuntu requirements"
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

X11_INPUT_IMAGE_INSTALL = " \
    xf86-input-tslib \
"
X11_VIDEO_ARMSOC = " \
    xf86-video-armsoc-nexell \
"

RDEPENDS_${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-mali', 'nexell-drm-mali-sato', '', d)}  \
    ${GSTREAMER10} \
    ${NEXELL_LIBS} \
    ${X11_INPUT_IMAGE_INSTALL} \
    ${X11_VIDEO_ARMSOC} \
"
