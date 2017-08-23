# Base this image on nexell-image-qt
require ../nexell-image-tiny.inc
inherit s5p4418-post-process

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

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

SMART_VOICE_APPS = " \
    smart-voice-testapp \
    "

TOOLS = " \
    perf \
    udev-extraconf \
    tzdata \
    net-tools \
    "

IMAGE_INSTALL += " \
    amixer-conf \
    rtl-8188eus-32 \
    tinyalsa \
    ${QT_ALSA_IMAGE_INSTALL} \
    ${QT_ALSA_EXTRA_IMAGE_INSTALL} \
    ${SYSTEMD_INSTALL} \
    ${TOOLS} \
    ${SMART_VOICE_APPS} \
    "
