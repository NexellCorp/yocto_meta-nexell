# Base this image on nexell-image-qt
require ../nexell-image-tiny.inc
inherit nexell-mali-postprocess
inherit s5p4418-post-process
inherit s5p4418-post-process-qt

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

COMPATIBLE_MACHINE = "s5p4418-smart-voice"

NEXELL_QT_APPS_INSTALL = " \
    NxAudioPlayer \
    NxQuickRearCam \
    NxVideoPlayer \
    "
WAYLAND_IMAGE_INSTALL = "${@base_contains('DISTRO_FEATURES', 'wayland', \
    'qtwayland \
     qtwayland-plugins', '', d)} \
    "
QT_IMAGE_INSTALL_common = " \
     qtbase \
     qtbase-plugins \
     qtdeclarative \
     qtmultimedia \
     qtmultimedia-plugins \
     qtmultimedia-qmlplugins \
     qtquickcontrols2 \
     ${WAYLAND_IMAGE_INSTALL} \
     "
QT_ALSA_IMAGE_INSTALL = " \
     alsa-utils \
     alsa-lib \
     "
QT_ALSA_EXTRA_IMAGE_INSTALL = " \
     alsa-utils-alsamixer \
     alsa-utils-midi \
     alsa-utils-aplay \
     alsa-utils-amixer \
     alsa-utils-aconnect \
     alsa-utils-iecset \
     alsa-utils-speakertest \
     alsa-utils-aseqnet \
     alsa-utils-aseqdump \
     alsa-utils-alsactl \
     "
SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
    "
QT_MULTIMEDIA = " \
    qtmultimedia \
    qtmultimedia-plugins \
    qtmultimedia-qmlplugins \
    "

IMAGE_INSTALL += " \
    amixer-conf \
    rtl-8188eus-32 \
    tinyalsa \
    nexell-drm-mali \
    weston-conf \
    nx-vidtex \
    packagegroup-nexell-common \
    ${NEXELL_QT_APPS_INSTALL} \
    ${WAYLAND_IMAGE_INSTALL} \
    ${QT_IMAGE_INSTALL_common} \
    ${QT_ALSA_IMAGE_INSTALL} \
    ${QT_ALSA_EXTRA_IMAGE_INSTALL} \
    ${SYSTEMD_INSTALL} \
    ${QT_MULTIMEDIA} \
    "
