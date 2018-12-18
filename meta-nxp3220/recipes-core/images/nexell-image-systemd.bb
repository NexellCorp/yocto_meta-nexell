inherit core-image
inherit post-process
include classes/nexell_rootfsimg.inc
include classes/nexell_dataimg.inc

LICENSE = "MIT"

export IMAGE_BASENAME = "nexell-image-systemd"

IMAGE_INSTALL_append = " \
    systemd-compat-units \
    rsyslog-systemd \
"
