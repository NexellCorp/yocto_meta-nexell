# Base this image on nexell-image-qt
include nexell-image-qt.inc
inherit s5p4418-avn-ref-post-process
inherit s5p4418-avn-ref-post-process-qt
inherit nexell-mali-postprocess

LICENSE = "LGPLv2.1"

IMAGE_INSTALL += "\    
    packagegroup-nexell-common \
    packagegroup-s5p4418-avn-ref-qt \
    "