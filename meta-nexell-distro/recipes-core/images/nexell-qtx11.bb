inherit post-process-qtx11
inherit post-process
require recipes-sato/images/core-image-sato.bb

DESCRIPTION ?= "Nexell qt images"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_FEATURES += "package-management ssh-server-dropbear splash"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

DEVEL_TOOLS = " \
    android-tools-nexell \
"

ETC_TOOLS = " \
    tinyalsa \
    udev-extraconf \
    tzdata \
    net-tools \
    openssl \
    curl \
"
GSTREAMER = " \
    gstreamer1.0 \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-base-alsa \
    gstreamer1.0-plugins-base-audioconvert \
    gstreamer1.0-plugins-base-audioresample \
    gstreamer1.0-plugins-base-audiotestsrc \
    gstreamer1.0-plugins-base-typefindfunctions \
    gstreamer1.0-plugins-base-ogg \
    gstreamer1.0-plugins-base-theora \
    gstreamer1.0-plugins-base-videotestsrc \
    gstreamer1.0-plugins-base-vorbis \
    gstreamer1.0-plugins-good-audioparsers \
    gstreamer1.0-plugins-good-autodetect \
    gstreamer1.0-plugins-good-avi \
    gstreamer1.0-plugins-good-deinterlace \
    gstreamer1.0-plugins-good-id3demux \
    gstreamer1.0-plugins-good-isomp4 \
    gstreamer1.0-plugins-good-matroska \
    gstreamer1.0-plugins-good-rtp \
    gstreamer1.0-plugins-good-rtpmanager \
    gstreamer1.0-plugins-good-udp \
    gstreamer1.0-plugins-good-video4linux2 \
    gstreamer1.0-plugins-good-wavenc \
    gstreamer1.0-plugins-good-wavparse \
"
X11_ESSENTIAL_IMAGES = " \
    xdg-utils \
    libxres \
    makedevs \
    mime-support \
    udev-extra-rules \
    connman connman-plugin-loopback connman-plugin-ethernet connman-plugin-wifi connman-client \
    xserver-nodm-init \
    xserver-xorg-extension-dbe \
    xserver-xorg-extension-extmod \
    xauth \
    xhost \
    xset \
    setxkbmap \
    xrdb \
    xorg-minimal-fonts xserver-xorg-utils \
    libxdamage libxvmc libxinerama \
    bash \
    v4l-utils \
    libpcre \
    libpcreposix \
    libxcomposite \
    alsa-states \
"

NEXELL_CUSTOMIZE_INSTALL = " \
    qt5-smarthome-launcher \
    nexell-qt5-touchsetup \
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
    ${DEVEL_TOOLS} \
    packagegroup-nexell-qt-x11 \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    ${X11_ESSENTIAL_IMAGES} \
    ${GSTREAMER} \
    qt5-x11-init \
"
