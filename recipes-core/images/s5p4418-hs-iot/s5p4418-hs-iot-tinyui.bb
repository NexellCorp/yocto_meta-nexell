# Base this image on nexell-image-qt5
require ../nexell-image-tinyui.inc
inherit s5p4418-hs-iot-post-process
inherit nexell-mali-postprocess

LICENSE = "LGPLv2.1"

IMAGE_INSTALL += "\    
    packagegroup-nexell-common \
    packagegroup-s5p4418-hs-iot-tinyui \
    "