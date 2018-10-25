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

EXTRA_ROOTFS = "/extra-rootfs-support"

do_install() {
    cd ${S}
    install -d ${D}${libdir}/nexell
    install -d ${D}${includedir}/nexell
    install -d ${D}${bindir}/nexell

    install -m 0644 ${S}/nexell/nexell_drm.h ${D}${includedir}/nexell/
    install -m 0644 ${S}/nexell/nexell_drmif.h ${D}${includedir}/nexell/

    install -m 0755 ${B}/nexell/.libs/libdrm_nexell.so.1.0.0 ${D}${libdir}/nexell/
    install -m 0755 ${B}/nexell/libdrm_nexell.la ${D}${libdir}/nexell/

    cd ${D}${libdir}
    ln -sf ./nexell/libdrm_nexell.so.1.0.0 libdrm_nexell.so.1
    ln -sf libdrm_nexell.so.1 libdrm_nexell.so

    install -m 0755 ${B}/tests/modetest/.libs/modetest ${D}${bindir}/nexell/

    # supported for ubuntu or fedora OS system
    install -d ${BASE_WORKDIR}${EXTRA_ROOTFS}${libdir}/nexell
    install -d ${BASE_WORKDIR}${EXTRA_ROOTFS}${includedir}/nexell
    install -d ${BASE_WORKDIR}${EXTRA_ROOTFS}${bindir}/nexell

    install -m 0644 ${S}/nexell/nexell_drm.h ${BASE_WORKDIR}${EXTRA_ROOTFS}${includedir}/nexell/
    install -m 0644 ${S}/nexell/nexell_drmif.h ${BASE_WORKDIR}${EXTRA_ROOTFS}${includedir}/nexell/

    install -m 0755 ${B}/nexell/.libs/libdrm_nexell.so.1.0.0 ${BASE_WORKDIR}${EXTRA_ROOTFS}${libdir}/nexell/
    install -m 0755 ${B}/nexell/libdrm_nexell.la ${BASE_WORKDIR}${EXTRA_ROOTFS}${libdir}/nexell/

    cd ${BASE_WORKDIR}${EXTRA_ROOTFS}${libdir}
    ln -sf ./nexell/libdrm_nexell.so.1.0.0 libdrm_nexell.so.1
    ln -sf libdrm_nexell.so.1 libdrm_nexell.so

    install -m 0755 ${B}/tests/modetest/.libs/modetest ${BASE_WORKDIR}${EXTRA_ROOTFS}${bindir}/nexell/
}

INSANE_SKIP_${PN} = "compile-host-path dev-so debug-files"
FILES_${PN} = "${libdir} ${libdir}/nexell ${includedir}/nexell ${bindir}/nexell"
FILES_${PN}-dev = "${includedir}/nexell"
ALLOW_EMPTY_${PN} = "1"
PROVIDES = "libdrm-nx"
