#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb

SUMMARY = "Minimal GUI requirements"
DESCRIPTION = "The minimal set of packages required by NEXELL Tiny GUI"
LICENSE = "MIT"

#PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = "\
    packagegroup-nexell-tinygui \
    "

RPROVIDES_${PN} = "virtual/egl virtual/libgles1 virtual/libgles2"
RDEPENDS_${PN} = "\
    clutter-example \
    mx-tests \
    mx-1.0 \
    "

#CORE_IMAGE_BASE_INSTALL += "clutter-1.0-examples"
#CORE_IMAGE_BASE_INSTALL += "clutter-1.0-examples mx-1.0 mx-tests"

#CORE_IMAGE_EXTRA_INSTALL += " \
#    ${@base_contains('DISTRO_FEATURES', 'directfb', 'packagegroup-core-directfb', '', d)} \
#    ${@base_contains('DISTRO_FEATURES', 'x11', '', \
#                       base_contains('DISTRO_FEATURES', 'wayland', \
#                                     'weston weston-init clutter-1.0-examples', '', d), d)} \
#"
#export IMAGE_BASENAME = "nexell-image-tiny-gui"