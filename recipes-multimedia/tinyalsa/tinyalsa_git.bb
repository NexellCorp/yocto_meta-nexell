SUMMARY = "tinyalsa"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://README.md;md5=4bc5db625003bf695c2356e82471b6f8"

SRCREV = "04fa39f287336c19953939f9523e04d0074a7f29"
SRC_URI = "git://github.com/tinyalsa/tinyalsa;protocol=git"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
    oe_runmake clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CPPFLAGS="${CFLAGS}" LDFLAGS+=" --sysroot=${STAGING_DIR_HOST} " CC="${CC}"
}

do_install () {
    oe_runmake install DESTDIR=${D}
}

INSANE_SKIP_${PN} = "installed-vs-shipped staticdev dev-so libdir"
INSANE_SKIP_${PN}-dbg += "libdir"
FILES_${PN} += "usr/local/bin usr/local/include/tinyalsa usr/local/lib usr/local/share/man/man1"
