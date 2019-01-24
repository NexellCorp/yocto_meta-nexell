inherit populate_sdk
inherit populate_sdk_qt5
require recipes-graphics/images/core-image-weston.bb

DESCRIPTION = "Nexell QT SDKs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

IMAGE_FEATURES += " dev-pkgs tools-sdk tools-debug debug-tweaks ssh-server-openssh"
SDKIMAGE_FEATURES += " dbg-pkgs staticdev-pkgs"

IMAGE_INSTALL_append = " \
    kernel-modules \
    packagegroup-nexell-common-sdk \
    packagegroup-nexell-qt-sdk \
"
