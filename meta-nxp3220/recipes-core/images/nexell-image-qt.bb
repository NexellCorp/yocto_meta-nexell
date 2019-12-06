inherit core-image
inherit nexell-rootfs
inherit nexell-rootfs-qt
inherit ${@bb.utils.contains('DISTRO_FEATURES', 'append-sdk', 'populate_sdk_qt5', '', d)}

include classes/nexell_image.inc

DESCRIPTION ?= "Nexell Qt images"
LICENSE = "MIT"

export IMAGE_BASENAME = "nexell-image-qt"

IMAGE_FEATURES += "package-management ssh-server-dropbear splash"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

UTILS_INSTALL = " \
    android-tools \
    udev-extraconf \
    libdrm-tests \
    strace \
    openssl \
    curl \
    procps \
    net-tools \
    tzdata \
    bash \
    dlt-daemon \
    ${@bb.utils.contains('IMAGE_FSTYPES', 'multiubi2', 'mtd-utils mtd-utils-ubifs', '', d)} \
"

LIBS_INSTALL = " \
    libdrm \
    dbus \
    libpcre \
    libpcreposix \
    id3lib \
    libid3tag \
    libjpeg-turbo \
    icu \
    libpng \
    protobuf-c \
"

MULTIMEDIA_INSTALL = " \
    alsa-utils \
    tinyalsa \
    ffmpeg \
    v4l-utils \
"

PACAKGEGROUPS_INSTALL = " \
    packagegroup-nxp3220-tslib \
    packagegroup-nxp3220-qt5.6 \
    packagegroup-nxp3220-gstreamer \
    packagegroup-nxp3220-libs \
    packagegroup-nxp3220-apps \
    packagegroup-nxp3220-qt-utils \
    packagegroup-nxp3220-testsuite \
    packagegroup-nxp3220-swupdate \
    ${@bb.utils.contains('DISTRO_FEATURES', 'support-directfb', 'packagegroup-nxp3220-directfb', '', d)} \
"

NEXELL_CUSTOMIZE_INSTALL = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'support-allgo-connectivity', 'common-api-c++-dbus common-api-c++', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'systemd support-daudio', 'nexell-qtbootlauncher', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'systemd support-daudio', 'nexell-nxlogrotate nexell-bluetooth', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'support-daudio support-daudio-sdk', 'nexell-daudio-sdk', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'support-daudio support-daudio-sdk support-allgo-connectivity support-allgo-connectivity-sdk', 'allgo-connectivity-sdk', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'support-nx-init', 'nexell-init', '', d)} \
    nexell-qt5-touchsetup \
    user-fonts \
"

IMAGE_INSTALL_append = " \
    ${SYSTEMD_INSTALL} \
    ${PACAKGEGROUPS_INSTALL} \
    ${MULTIMEDIA_INSTALL} \
    ${UTILS_INSTALL} \
    ${LIBS_INSTALL} \
    ${NEXELL_CUSTOMIZE_INSTALL} \
    kernel-modules \
"
