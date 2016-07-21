# Base this image on nexell-image-qt5
include nexell-image-qt5.inc
inherit s5p4418-avn-ref-post-process
inherit s5p4418-avn-ref-post-process-qt
inherit nexell-mali-postprocess

LICENSE = "LGPLv2.1"

IMAGE_INSTALL_append = "packagegroup-s5p4418-avn-ref-qt"