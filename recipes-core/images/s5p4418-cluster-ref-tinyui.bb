# Base this image on nexell-image-qt5
include nexell-image-tinyui.inc
inherit s5p4418-cluster-ref-post-process
inherit nexell-mali-postprocess

LICENSE = "LGPLv2.1"

IMAGE_INSTALL += "\    
    packagegroup-nexell-common \
    packagegroup-s5p4418-cluster-ref-tinyui \
    "
