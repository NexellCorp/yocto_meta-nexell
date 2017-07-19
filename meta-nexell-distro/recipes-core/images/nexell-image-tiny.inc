# Base this image on core-image-minimal

include recipes-core/images/core-image-minimal.bb
LICENSE = "GPLv2"

IMAGE_FEATURES += " splash"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

IMAGE_INSTALL_append= " \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
"



