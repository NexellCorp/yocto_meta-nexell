DESCRIPTION = "hwio"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=1c29f522bb1f7e5686af40aff4032b50"

SRC_URI = "git://review.gerrithub.io/NexellCorp/linux_apps_hwio;protocol=https;branch=nexell"
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

PV ?= "1.0+git${SRCPV}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit autotools pkgconfig

EXTRA_OECONF = "'--prefix=${STAGING_DIR_HOST}'"
EXTRA_OEMAKE = "$(WARN_CFLAGS)"

# Fix build error when EXTERNALSRC equal EXTERNALSRC_BUILD
# Error : source directory already configured
do_configure_prepend() {
    if [ -f ${S}/config.status ]; then
	oe_runmake -C ${S} distclean
    fi
}

INSANE_SKIP_${PN} = "installed-vs-shipped"
FILES_${PN} = "${bindir}"
ALLOW_EMPTY_${PN} = "1"
