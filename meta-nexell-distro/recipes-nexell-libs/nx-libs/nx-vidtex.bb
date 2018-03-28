inherit linux-nexell-base

DESCRIPTION = "nx-vidtex"
LICENSE = "CLOSED"

SRC_URI = "file://lib \
          "

S = "${WORKDIR}"

PV = "1"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

NEED_TO_EXTRA_ROOTFS_SUPPORT = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-ubuntu', 'true', 'false', d)}"

do_install() {
    install -d ${D}${libdir}
    install -m 0644 ${S}/lib/libnxvidtex.so-${ARCH_TYPE_NUM} ${D}${libdir}/libnxvidtex.so

    if [ ${NEED_TO_EXTRA_ROOTFS_SUPPORT} == "true" ]; then
       install -m 0644 ${S}/lib/libnxvidtex.so-${ARCH_TYPE_NUM} ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/libnxvidtex.so
    fi
}

INSANE_SKIP_${PN} = "dev-so ldflags invalid-packageconfig"
FILES_${PN} = "${libdir}"
FILES_SOLIBSDEV = ""