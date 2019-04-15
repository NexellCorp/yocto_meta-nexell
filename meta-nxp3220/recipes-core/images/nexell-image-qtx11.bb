require recipes-sato/images/core-image-sato.bb
inherit nexell-rootfs
inherit nexell-rootfs-qtx11
include classes/nexell_rootfsimg.inc
include classes/nexell_dataimg.inc

DESCRIPTION ?= "Nexell Qt-X11 images"
LICENSE = "MIT"

export IMAGE_BASENAME = "nexell-image-qtx11"

IMAGE_FEATURES += "package-management ssh-server-dropbear splash"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

UTILS_INSTALL = " \
    android-tools \
    udev-extraconf \
    libdrm \
    libdrm-tests \
    tinyalsa \
    strace \
"

MULTIMEDIA_INSTALL = " \
    ffmpeg \
"

PACAKGEGROUPS_INSTALL = " \
    packagegroup-nxp3220-alsa-utils \
    packagegroup-nxp3220-tslib \
    packagegroup-nxp3220-x11 \
    packagegroup-nxp3220-qt5.6 \
    packagegroup-nxp3220-gstreamer \
    packagegroup-nxp3220-libs \
"

IMAGE_INSTALL_append = " \
    ${SYSTEMD_INSTALL} \
    ${PACAKGEGROUPS_INSTALL} \
    ${MULTIMEDIA_INSTALL} \
    ${UTILS_INSTALL} \
"