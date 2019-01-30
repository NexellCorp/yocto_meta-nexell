# Copyright (C) 2015 Romain Perier <romain.perier@gmail.com>
# Released under the MIT license (see COPYING.MIT for the terms)
require recipes-graphics/xorg-driver/xorg-driver-video.inc

DESCRIPTION = "X.org graphics driver for ARM graphics - Nexell"
LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://COPYING;md5=10ce5de3b111315ea652a5f74ec0c602"

PV ?= "1.0+git${SRCPV}"
SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/linux_library_xf86-video-armsoc;protocol=https;branch=nexell"
SRC_URI_append = " \
        file://20-armsoc.conf \
        file://90-armsoc.conf \
        file://99-calibration-nexell.conf \
        "

RDEPENDS_${PN} += "xserver-xorg-module-exa"

S = "${WORKDIR}/git"

GLX_USE = "${@bb.utils.contains('DISTRO_FEATURES', 'qt-default-platform-x11', 'enable-glx', 'disable-glx', d)}"

do_install_append() {
    install -d ${D}/etc/X11/xorg.conf.d
    if [ ${GLX_USE} == "disable-glx" ]; then
        install -m 644 ${WORKDIR}/20-armsoc.conf ${D}/etc/X11/xorg.conf.d/
        install -m 644 ${WORKDIR}/99-calibration-nexell.conf ${D}/etc/X11/xorg.conf.d/99-calibration.conf
    else
        install -m 644 ${WORKDIR}/90-armsoc.conf ${D}/etc/X11/xorg.conf.d/
    fi
}

FILES_${PN} += "${sysconfdir}"
