# artik7 BSP default providers

PREFERRED_PROVIDER_virtual/kernel ?= "linux-artik7"
PREFERRED_VERSION_linux-nexell ?= "4.1.%"

PREFERRED_PROVIDER_virtual/libgles2 = "mali-nexell"
PREFERRED_PROVIDER_virtual/egl = "mali-nexell"
PREFERRED_PROVIDER_virtual/libgles1 = "mali-nexell"
PREFERRED_PROVIDER_virtual/libopencl ?= "mesa-gl"
PREFERRED_PROVIDER_virtual/libgl ?= "mesa-gl"
PREFERRED_PROVIDER_virtual/mesa ?= "mesa-gl"

MULTILIBS = "multilib:lib32 multilib:lib64"

TINY_GUI_DEFAULT_DISTRO_FEATURES = ""
TINY_GUI_DEFAULT_EXTRA_RDEPENDS = "packagegroup-core-boot"

DISTRO_FEATURES ?= "${DISTRO_FEATURES_DEFAULT} ${DISTRO_FEATURES_LIBC} ${TINY_GUI_DEFAULT_DISTRO_FEATURES}"

DISTRO_EXTRA_RDEPENDS += "${TINY_GUI_DEFAULT_EXTRA_RDEPENDS}"

SDK_NAME = "${DISTRO}-${TCLIBC}-${SDK_ARCH}-${IMAGE_BASENAME}-${TUNE_PKGARCH}"
SDKPATH = "/opt/${DISTRO}/${SDK_VERSION}"

TCLIBCAPPEND = ""
