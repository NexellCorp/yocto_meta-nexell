# Base this image on nexell-image-qt5
require ../nexell-image-tinyui.inc
require ../nexell-image-common.inc
inherit s5p4418-post-process
inherit nexell-mali-postprocess
inherit s5p4418-post-process-cluster-sdk

LICENSE = "LGPLv2.1"

IMAGE_INSTALL += " \
    packagegroup-nexell-common \
    packagegroup-s5p4418-tinyui \
	cluster-init \
    "
