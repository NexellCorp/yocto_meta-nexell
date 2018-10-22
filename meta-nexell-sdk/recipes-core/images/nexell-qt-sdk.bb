inherit populate_sdk_qt5
require recipes-graphics/images/core-image-weston.bb

DESCRIPTION = "Nexell QT SDKs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_FEATURES += " dev-pkgs tools-sdk tools-debug debug-tweaks ssh-server-openssh"
SDKIMAGE_FEATURES += " dbg-pkgs staticdev-pkgs"

ALLGO_CONNECTIVITY = "${@bb.utils.contains('DISTRO_FEATURES', 'support-allgo-connectivity', 'common-api-c++-dbus common-api-c++', '', d)}"


NEXELL_CUSTOMIZE_INSTALL = " \
    ${ALLGO_CONNECTIVITY} \
"

NEXELL_LIBRARY_INSTALL = " \
    nx-drm-allocator \
    nx-renderer \
    nx-gst-meta \
    nx-scaler \
    nx-v4l2 \
    nx-video-api \
    nx-vidtex \
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    packagegroup-nexell-common-sdk \
    packagegroup-nexell-qt-sdk \
    ${NEXELL_LIBRARY_INSTALL} \
    ${NEXELL_CUSTOMIZE_INSTALL} \
"
