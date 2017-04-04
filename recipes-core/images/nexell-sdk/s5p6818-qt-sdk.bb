# Base this image on nexell-image-qt
require ../nexell-image-qt.inc
require ../nexell-image-common.inc

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_INSTALL += " \
    packagegroup-nexell-common \
    packagegroup-s5p6818-qt \
    packagegroup-nexell-sdk \
    "