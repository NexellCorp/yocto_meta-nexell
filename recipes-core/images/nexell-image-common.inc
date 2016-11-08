DESCRIPTION ?= "Nexell common images"
LICENSE ?= "GPLv2"

SPLASH ?= "psplash-nexell"

IMAGE_FEATURES += "package-management ssh-server-dropbear splash"

TOOLS = " \
    perf \
    udev-extraconf \
    "

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
    "

SYSV_INSTALL = " \
    udev \
    sysvinit \
    initscripts \
    "

IMAGE_INSTALL += " \
    kernel-modules \
    android-tools \
    ${SYSTEMD_INSTALL} \
    ${TOOLS} \
    "
