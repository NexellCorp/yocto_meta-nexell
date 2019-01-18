inherit core-image
inherit nexell-rootfs
inherit nexell-rootfs-qt
inherit ${@bb.utils.contains('DISTRO_FEATURES', 'append-sdk', 'populate_sdk', '', d)}
inherit ${@bb.utils.contains('DISTRO_FEATURES', 'append-sdk', 'populate_sdk_qt5', '', d)}

include classes/nexell_rootfsimg.inc
include classes/nexell_dataimg.inc

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
"

MULTIMEDIA_INSTALL = " \
    tinyalsa \
    ffmpeg \
    v4l-utils \
"

PACAKGEGROUPS_INSTALL = " \
    packagegroup-nxp3220-alsa-utils \
    packagegroup-nxp3220-tslib \
    packagegroup-nxp3220-qt5.6 \
    packagegroup-nxp3220-gstreamer \
    packagegroup-nxp3220-libs \
    packagegroup-nxp3220-qt-utils \
"

NEXELL_CUSTOMIZE_INSTALL = " \
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
"
