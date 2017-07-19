require nexell-image-qt.inc
inherit post-process-qt
inherit post-process

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_INSTALL_append = " \
    packagegroup-nexell-qt \
    rtl-8188eus-${ARCH_TYPE_NUM} \
    testsuite-${NEXELL_BOARD_SOCNAME} \
"
