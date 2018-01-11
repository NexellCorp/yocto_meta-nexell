require recipes-graphics/images/core-image-weston.bb

DESCRIPTION = "Nexell QT SDKs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_FEATURES += " dev-pkgs tools-sdk tools-debug debug-tweaks ssh-server-openssh"
SDKIMAGE_FEATURES += " dbg-pkgs staticdev-pkgs"

TOUCH_IMAGE_INSTALL = " \
    tslib \
    tslib-conf \
    tslib-tests \
    tslib-calibrate \
    tslib-nexell \
"

NEXELL_CUSTOMIZE_INSTALL = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-daudio', 'qtbootlauncher', \
                              bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', 'qt5-smarthome-launcher', 'qtbootlauncher', d), d)} \
    nexell-qt5-touchsetup \
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    packagegroup-nexell-common-sdk \
    packagegroup-nexell-qt-sdk \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    ${TOUCH_IMAGE_INSTALL} \
    ${NEXELL_CUSTOMIZE_INSTALL} \
"
