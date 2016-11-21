# Base this image on nexell-image-qt
require ../nexell-image-qt.inc
require ../nexell-image-common.inc
inherit nexell-mali-postprocess
inherit s5p4418-post-process
inherit s5p4418-post-process-qt

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

COMPATIBLE_MACHINE = "s5p4418-cluster-ref"

IMAGE_INSTALL += " \
    packagegroup-nexell-common \
    packagegroup-s5p4418-qt \
    "

