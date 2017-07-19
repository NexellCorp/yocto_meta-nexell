include recipes-core/images/core-image-minimal.bb
inherit post-process
inherit post-process-qt

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

SMART_VOICE_APPS = " \
    smart-voice-testapp \
"

DEVEL_TOOLS = " \
    android-tools-nexell \
"

ETC_TOOLS = " \
    amixer-conf \
    tinyalsa \
    udev-extraconf \
    tzdata \
    net-tools \
    openssl \
    curl \
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    nx-vidtex \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    packagegroup-nexell-smartvoice \
    ${SYSTEMD_INSTALL} \
    ${SMART_VOICE_APPS} \
    ${DEVEL_TOOLS} \
    ${ETC_TOOLS} \
"
