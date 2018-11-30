inherit core-image
inherit post-process
include classes/nexell_rootfsimg.inc

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

export IMAGE_BASENAME = "nexell-image-systemd"

IMAGE_INSTALL_append = " \
    systemd-compat-units \
    rsyslog-systemd \
"
