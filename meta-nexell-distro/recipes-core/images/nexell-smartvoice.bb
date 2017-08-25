inherit core-image
inherit post-process

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

ETC_TOOLS = " \
    amixer-conf \
    tinyalsa \
    net-tools \
"

IMAGE_INSTALL_append = " \
    kernel-modules \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    ${ETC_TOOLS} \
"
