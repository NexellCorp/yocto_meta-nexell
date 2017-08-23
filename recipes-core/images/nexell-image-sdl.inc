DESCRIPTION = "Extend core-image by adding sdl"
LICENSE = "GPLv2"

inherit core-image
include recipes-core/images/core-image-minimal.bb

TOUCH_IMAGE_INSTALL = " \
    tslib \
    tslib-conf \
    tslib-tests \
    tslib-calibrate \
    tslib-nexell \
    "

SDL_IMAGE_INSTALL = " \
    libsdl2 \
    "

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
    "

SYSV_IMAGE_INSTALL = " \
    udev \
    sysvinit \
    initscripts \
    "

MULTIMEDIA_IMAGE_INSTALL = " \
    pulseaudio \
    pulseaudio-server \
    ffmpeg \
    fswebcam \
    "

#NEXELL_LIBS_EXT = " \
#    nx-vidtex \
#    "

IMAGE_INSTALL += " \
     kernel-modules \
     ${SYSTEMD_INSTALL} \
     ${TOUCH_IMAGE_INSTALL} \
     ${MULTIMEDIA_IMAGE_INSTALL} \
     "
