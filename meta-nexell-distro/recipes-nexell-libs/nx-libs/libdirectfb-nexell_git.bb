DESCRIPTION = "libdirectfb nexell"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=1c29f522bb1f7e5686af40aff4032b50"

SRC_URI = "git://review.gerrithub.io/NexellCorp/linux_library_libdirectfb-nexell;protocol=https;branch=nexell"
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

PV ?= "1.0+git${SRCPV}"

PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "(nxp3220)"

DEPENDS = "libdrm directfb"

inherit autotools pkgconfig

GFXDRIVERS_MODULEDIR = "${libdir}/directfb-1.7-7"

EXTRA_OECONF = " \
    --host=${HOST_SYS} \
    --target=${TARGET_SYS} \
    --prefix=${STAGING_INCDIR} \
    --includedir=${STAGING_INCDIR} \
    "

EXTRA_OEMAKE = " \
    'CFLAGS+=-I${STAGING_KERNEL_DIR}/include/uapi/drm' \
    'MODULEDIR=${GFXDRIVERS_MODULEDIR}' \
"

INSANE_SKIP_${PN} = "dev-so invalid-packageconfig"
FILES_${PN} = "${GFXDRIVERS_MODULEDIR}/* "
RDEPENDS_${PN} += "libdrm directfb"
ALLOW_EMPTY_${PN} = "1"
