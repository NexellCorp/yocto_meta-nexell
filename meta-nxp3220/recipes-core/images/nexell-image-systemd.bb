inherit core-image
inherit nexell-rootfs
include classes/nexell_rootfsimg.inc
include classes/nexell_dataimg.inc

LICENSE = "MIT"

export IMAGE_BASENAME = "nexell-image-systemd"

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

PACAKGEGROUPS_INSTALL = " \
    packagegroup-nxp3220-alsa-utils \
    packagegroup-nxp3220-tslib \
"

IMAGE_INSTALL_append = " \
    ${SYSTEMD_INSTALL} \
    ${UTILS_INSTALL} \
    ${PACAKGEGROUPS_INSTALL} \
"
