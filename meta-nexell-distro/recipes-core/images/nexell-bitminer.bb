inherit post-process
include recipes-core/images/core-image-minimal.bb

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_FEATURES += " splash"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

CGMINER_INSTALL = " \
    cgminer-gekko \
"

WEB_INSTALL = " \
	libmodbus \
	modbus-tcp-server \
    lighttpd \
"

NEXELL_KERNEL_TEST_IMAGE_INSTALL = " \
    can-utils \
"

NAVI_REF_WIFI_INSTALL = " \
    kernel-modules \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    wireless-tools \
    wpa-supplicant \
"

IMAGE_INSTALL_append = " \
    net-tools \
    spidevtest \
    ${@bb.utils.contains('MACHINE', 's5p4418-navi-ref', '${NAVI_REF_WIFI_INSTALL}', '', d)} \
    ${@bb.utils.contains('MACHINE', 's5p4418-bitminer-ref', '${NAVI_REF_WIFI_INSTALL}', '', d)} \
    ${@bb.utils.contains('MACHINE', 's5p6818-bitminer-ref', '${NAVI_REF_WIFI_INSTALL}', '', d)} \
    ${WEB_INSTALL} \
    ${CGMINER_INSTALL} \
    ${SYSTEMD_INSTALL} \
    ${NEXELL_KERNEL_TEST_IMAGE_INSTALL} \
"

