# Copyright (C) 2015 Romain Perier <romain.perier@gmail.com>
# Released under the MIT license (see COPYING.MIT for the terms)
require recipes-graphics/xorg-driver/xorg-driver-video.inc

DESCRIPTION = "X.org graphics driver for ARM graphics - Nexell"
LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://COPYING;md5=10ce5de3b111315ea652a5f74ec0c602"

SRCREV = "79b8d6961a90d976a0468614d3366e54d6e53a3e"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/xf86-video-armsoc;protocol=git;branch=nexell \
          "
SRC_URI_append = " \
        file://20-armsoc.conf \
        file://99-calibration-nexell.conf \
        "

PV = "r7"

RDEPENDS_${PN} += "xserver-xorg-module-exa"

S = "${WORKDIR}/git"

do_install_append() {
    install -d ${D}/etc/X11/xorg.conf.d
    install -m 644 ${WORKDIR}/20-armsoc.conf ${D}/etc/X11/xorg.conf.d
    install -m 644 ${WORKDIR}/99-calibration-nexell.conf ${D}/etc/X11/xorg.conf.d/99-calibration.conf
}

FILES_${PN} += "${sysconfdir}"