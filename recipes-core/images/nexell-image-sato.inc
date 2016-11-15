# Base this image on core-image-sato
include recipes-sato/images/core-image-sato.bb
LICENSE = "GPLv2"

TOUCH_IMAGE_INSTALL = " \
     tslib \
     tslib-conf \
     tslib-tests \
     tslib-calibrate \
     tslib-nexell \
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
    ${TOUCH_IMAGE_INSTALL} \
    ${X11_ALSA_IMAGE_INSTALL} \
    ${X11_ALSA_EXTRA_IMAGE_INSTALL} \
    "