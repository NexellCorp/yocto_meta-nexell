# Base this image on core-image-minimal
include recipes-core/images/core-image-minimal.bb
LICENSE = "GPLv2"

SPLASH = "psplash-nexell"

IMAGE_FEATURES += "splash"

# Include modules in rootfs
IMAGE_INSTALL += " \
              kernel-modules \
	      "