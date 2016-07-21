#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "QT requirements"
DESCRIPTION = "The qt set of packages required by NEXELL AVN"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = " \
    ${PN} \
    "
    
RDEPENDS_${PN} = " \
    gstreamer1.0 \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    "

RDEPENDS_${PN} += " \
    helloworld-nexell \
    helloworld-nexell-module \
    gst-plugins-camera-s5p6818 \
    nx-drm-allocator \
    nx-gst-meta \
    nx-v4l2 \
    example-libs \
    "

RDEPENDS_${PN} += " \
    wayland-drm-s5p4418 \
    weston-conf \
    mx-1.0 \
    mx-tests \
    "