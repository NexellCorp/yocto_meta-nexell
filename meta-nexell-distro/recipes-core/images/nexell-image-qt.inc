DESCRIPTION ?= "Nexell common images"
LICENSE ?= "GPLv2"

require recipes-graphics/images/core-image-weston.bb

IMAGE_FEATURES += "package-management ssh-server-dropbear splash"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

DEVEL_TOOLS = " \
    android-tools-nexell \
"

PODO_IMAGES_INSTALL = " \
     podonamu \
     connman \
     connman-client \
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
    ${DEVEL_TOOLS} \
"
#${PODO_IMAGES_INSTALL}