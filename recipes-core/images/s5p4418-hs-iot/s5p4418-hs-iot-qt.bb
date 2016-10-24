# Base this image on nexell-image-qt
require ../nexell-image-qt.inc
inherit nexell-mali-postprocess
inherit s5p4418-hs-iot-post-process
inherit s5p4418-hs-iot-post-process-qt

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=4d92cd373abda3937c2bc47fbc49d690"

COMPATIBLE_MACHINE = "s5p4418-hs-iot"

IMAGE_INSTALL += "\    
    packagegroup-nexell-common \
    packagegroup-s5p4418-hs-iot-qt \
    "
