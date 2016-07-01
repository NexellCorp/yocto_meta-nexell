# Base this image on nexell-image-qt5
require recipes-graphics/images/core-image-weston.bb
include nexell-image-qt5.inc
inherit nexell-post-process-avn-4418
inherit nexell-post-process-avn-qt

LICENSE = "LGPLv2.1"
