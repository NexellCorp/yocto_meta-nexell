inherit core-image
inherit nexell-rootfs
include classes/nexell_image.inc

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
    ${@bb.utils.contains('IMAGE_FSTYPES', 'multiubi2', 'mtd-utils mtd-utils-ubifs', '', d)} \
"

PACAKGEGROUPS_INSTALL = " \
    packagegroup-nxp3220-alsa-utils \
    packagegroup-nxp3220-tslib \
    packagegroup-nxp3220-testsuite \
    packagegroup-nxp3220-swupdate \
"

IMAGE_INSTALL_append = " \
    ${SYSTEMD_INSTALL} \
    ${UTILS_INSTALL} \
    ${PACAKGEGROUPS_INSTALL} \
"
