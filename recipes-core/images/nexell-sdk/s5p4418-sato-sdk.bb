# Base this image on nexell-image-sato
require ../nexell-image-sato.inc
require ../nexell-image-common.inc

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_INSTALL += " \
    packagegroup-s5p4418-sato \
    packagegroup-nexell-common \
    packagegroup-nexell-sdk \
    "

