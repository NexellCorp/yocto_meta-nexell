# Base this image on core-image-sato
include recipes-sato/images/core-image-sato.bb
LICENSE = "GPLv2"

#X11 touch calibration
#==> /usr/bin# DISPLAY=:0.0 xinput_calibrator
X11_INPUT_IMAGE_INSTALL = " \
    xf86-input-tslib \
    "
X11_VIDEO_ARMSOC = " \
    xf86-video-armsoc-nexell \
    "
X11_EXTENSION = " \
    xserver-xorg-extension-glx \
    "
X11_ALSA_IMAGE_INSTALL = " \
    alsa-utils \
    alsa-lib \
    "
X11_ALSA_EXTRA_IMAGE_INSTALL = " \
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

IMAGE_INSTALL += " \
    ${X11_INPUT_IMAGE_INSTALL} \
    ${X11_VIDEO_ARMSOC} \
    ${X11_EXTENSION} \
    ${X11_ALSA_IMAGE_INSTALL} \
    ${X11_ALSA_EXTRA_IMAGE_INSTALL} \
    "