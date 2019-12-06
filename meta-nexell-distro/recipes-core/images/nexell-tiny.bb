inherit post-process
include recipes-core/images/core-image-minimal.bb

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

IMAGE_FEATURES += " splash"

SYSTEMD_INSTALL = " \
    systemd-compat-units \
    rsyslog-systemd \
"

DEVEL_TOOLS = " \
    android-tools-nexell \
    procps \
    net-tools \
    iperf3 \
    ethtool \
    udev-extraconf \
"

NEXELL_KERNEL_TEST_IMAGE_INSTALL = " \
    can-utils \
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    ${SYSTEMD_INSTALL} \
    ${NEXELL_KERNEL_TEST_IMAGE_INSTALL} \
    ${DEVEL_TOOLS} \
    testsuite \
"
