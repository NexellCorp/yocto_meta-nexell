# Base this image on nexell-image-qt
require ../nexell-image-qt.inc
require ../nexell-image-common.inc
inherit nexell-mali-postprocess
inherit s5p6818-post-process
inherit s5p6818-post-process-qt

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_INSTALL += " \
    optee-linuxdriver \
    optee-build \
    "

IMAGE_INSTALL += " \
    packagegroup-nexell-common \
    packagegroup-s5p6818-qt \
    "
