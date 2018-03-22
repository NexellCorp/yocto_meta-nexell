# Base this image on nexell-image-sato
require ../nexell-image-sato.inc
require ../nexell-image-common.inc
inherit s5p4418-post-process
inherit s5p4418-post-process-sato

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

COMPATIBLE_MACHINE = "s5p4418-cluster-ref"

IMAGE_INSTALL += " \
    packagegroup-s5p4418-sato \
    packagegroup-nexell-common \
    "

