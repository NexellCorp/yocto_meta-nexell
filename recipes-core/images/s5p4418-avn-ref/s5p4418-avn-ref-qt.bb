# Base this image on nexell-image-qt
require ../nexell-image-qt.inc
inherit s5p4418-avn-ref-post-process
inherit s5p4418-avn-ref-post-process-qt
inherit nexell-mali-postprocess

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

IMAGE_INSTALL += "\    
    packagegroup-nexell-common \
    packagegroup-s5p4418-avn-ref-qt \
    "