# Base this image on nexell-image-qt5
require recipes-graphics/images/core-image-weston.bb
include nexell-image-qt5.inc
inherit nexell-post-process-avn

LICENSE = "LGPLv2.1"
