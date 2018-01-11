inherit core-image
inherit post-process
include recipes-core/images/core-image-minimal.bb

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"
DEVEL_TOOLS = " \
    android-tools-nexell \
"
ETC_TOOLS = " \
    amixer-conf \
    tinyalsa \
    net-tools \
"
SMART_VOICE_APPS = " \
    smart-voice-testapp \
"
IMAGE_INSTALL_append = " \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    packagegroup-nexell-smartvoice \
    ${ETC_TOOLS} \
    ${DEVEL_TOOLS} \
    ${SMART_VOICE_APPS} \
"
