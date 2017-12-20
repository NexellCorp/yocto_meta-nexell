DESCRIPTION ?= "Nexell common images"
LICENSE ?= "GPLv2"

SPLASH ?= "psplash-nexell"

IMAGE_FEATURES += "package-management ssh-server-dropbear splash"

TOOLS = " \
    perf \
    udev-extraconf \
    tzdata \
    net-tools \
    "

GLIBC_INSTALL = " \
    glibc-utils \
    glibc-localedata-i18n \
    glibc-gconv-ibm850 \
    glibc-gconv-cp1252 \
    glibc-gconv-iso8859-1 \
    glibc-gconv-iso8859-15 \
    glibc-gconv-euc-kr \
    locale-base-en-gb \
    localedef \
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

NEXELL_LIBS_EXT = " \
    nx-vidtex \
    "

NEXELL_MULTIMEDIA = " \
    tinyalsa \
    "

IMAGE_INSTALL += " \
    kernel-modules \
    android-tools \
    ${SYSTEMD_INSTALL} \
    ${TOOLS} \
    ${NEXELL_LIBS_EXT} \
    ${GLIBC_INSTALL} \
    ${NEXELL_MULTIMEDIA} \
    nexell-nxreboot \
    "
