#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb
# add :
# IMAGE_FEATURES += "package-management"
# IMAGE_INSTALL_append = "packagegroup-<name>"

SUMMARY = "Alsa utils requirements"
DESCRIPTION = "The alsa utils packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGE_ALSA_UTILS = " \
    alsa-utils \
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
RDEPENDS_${PN} = " \
    ${PACKAGE_ALSA_UTILS} \
"
