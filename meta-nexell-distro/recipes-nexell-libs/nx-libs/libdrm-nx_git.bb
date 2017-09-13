DESCRIPTION = "nexell libdrm"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=b8808a8f91016e96e050886b9076009a"

SRCREV = "5017ac7f9cf4aaf56f3761c662e72e517b05de91"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/libdrm;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "gstreamer1.0 gstreamer1.0-plugins-base glib-2.0 libdrm"

inherit autotools pkgconfig

EXTRA_OECONF += "\
 --prefix=${PKG_CONFIG_SYSROOT_DIR} \
 --enable-nexell \
 --disable-tegra-experimental-api \
 --disable-udev \
 --disable-valgrind \
 --enable-install-test-programs \
 --disable-dependency-tracking \
 --enable-static \
 --enable-shared \
 --disable-cairo-tests \
 --disable-manpages \
 --disable-intel \
 --disable-radeon \
 --disable-amdgpu \
 --disable-nouveau \
 --disable-vmwgfx \
 --disable-omap-experimental-api \
 --disable-exynos-experimental-api \
 --disable-freedreno \
"

do_install() {
    cd ${S}
    install -d ${D}${libdir}/nexell
    install -d ${D}${includedir}/nexell
    install -d ${D}/lib/nexell
    install -d ${D}${bindir}

    install -m 0644 ${S}/nexell/nexell_drm.h ${D}${includedir}/nexell
    install -m 0644 ${S}/nexell/nexell_drmif.h ${D}${includedir}/nexell

    install -m 0755 ${B}/nexell/.libs/libdrm_nexell.so.1.0.0 ${D}/lib/nexell
    install -m 0755 ${B}/nexell/libdrm_nexell.la ${D}/lib/nexell

    cd ${D}/lib
    ln -sf ./nexell/libdrm_nexell.so.1.0.0 libdrm_nexell.so
    ln -sf ./nexell/libdrm_nexell.so.1.0.0 libdrm_nexell.so.1

    install -m 0755 ${B}/tests/modetest/.libs/modetest ${D}${bindir}

    # supported for ubuntu or fedora OS system
    install -d ${BASE_WORKDIR}/extra-rootfs-support/usr/lib/nexell
    install -d ${BASE_WORKDIR}/extra-rootfs-support/usr/include/nexell
    install -d ${BASE_WORKDIR}/extra-rootfs-support/lib/nexell
    install -d ${BASE_WORKDIR}/extra-rootfs-support/usr/bin

    install -m 0644 ${S}/nexell/nexell_drm.h ${BASE_WORKDIR}/extra-rootfs-support/usr/include/nexell/
    install -m 0644 ${S}/nexell/nexell_drmif.h ${BASE_WORKDIR}/extra-rootfs-support/usr/include/nexell/

    install -m 0755 ${B}/nexell/.libs/libdrm_nexell.so.1.0.0 ${BASE_WORKDIR}/extra-rootfs-support/lib/nexell/
    install -m 0755 ${B}/nexell/libdrm_nexell.la ${BASE_WORKDIR}/extra-rootfs-support/lib/nexell/

    cd ${BASE_WORKDIR}/extra-rootfs-support/lib
    ln -sf ./nexell/libdrm_nexell.so.1.0.0 libdrm_nexell.so
    ln -sf ./nexell/libdrm_nexell.so.1.0.0 libdrm_nexell.so.1

    install -m 0755 ${B}/tests/modetest/.libs/modetest ${BASE_WORKDIR}/extra-rootfs-support/usr/bin/
}

INSANE_SKIP_${PN} = "compile-host-path dev-so debug-files"
FILES_${PN} = "${libdir} ${includedir} lib lib/nexell ${bindir}"
ALLOW_EMPTY_${PN} = "1"
PROVIDES = "libdrm-nx"
