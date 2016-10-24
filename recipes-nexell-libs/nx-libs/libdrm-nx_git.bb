DESCRIPTION = "nexell libdrm"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile.am;md5=b8808a8f91016e96e050886b9076009a"

SRCREV = "519b955d4159033815cf9caf4a65846059561c9b"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/libdrm;protocol=git;branch=yocto-hs-iot"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "gstreamer1.0 gstreamer1.0-plugins-base glib-2.0 libomxil-nx libdrm"

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

do_configure() {
    cd ${S}
    NOCONFIGURE=true ./autogen.sh
    oe_runconf
}

do_compile() {
    cd ${S}
    oe_runmake clean
    oe_runmake
}

# do_install_prepend() {
#     rm -rf ${D}${includedir}/nexell
#     rm -rf ${D}${libdir}
#     rm -rf ${PKG_CONFIG_SYSROOT_DIR}/usr/include/libdrm
#     rm -rf ${PKG_CONFIG_SYSROOT_DIR}/usr/include/xf86drmMode.h
#     rm -rf ${PKG_CONFIG_SYSROOT_DIR}/usr/include/xf86drm.h
#     rm -rf ${PKG_CONFIG_SYSROOT_DIR}/usr/include/libkms/libkms.h
#     rm -rf ${PKG_CONFIG_SYSROOT_DIR}/usr/lib/libkms*
#     rm -rf ${PKG_CONFIG_SYSROOT_DIR}/usr/lib/libdrm*
#     rm -rf ${PKG_CONFIG_SYSROOT_DIR}/usr/lib/pkgconfig/libdrm.pc
#     rm -rf ${PKG_CONFIG_SYSROOT_DIR}/usr/lib/pkgconfig/libkms.pc
# }

do_install() {
    cd ${S}
    install -d ${D}${libdir}/nexell
    install -d ${D}${includedir}/nexell
    install -d ${D}/lib/nexell
    install -d ${D}${bindir}
#    oe_runmake install DESTDIR=${D}

    install -m 0644 ${S}/nexell/nexell_drm.h ${D}${includedir}/nexell
    install -m 0644 ${S}/nexell/nexell_drmif.h ${D}${includedir}/nexell	

    install -m 0755 ${S}/nexell/.libs/libdrm_nexell.so.1.0.0 ${D}/lib/nexell
    install -m 0755 ${S}/nexell/libdrm_nexell.la ${D}/lib/nexell
#    ${STAGING_BASELIBDIR}										

    cd ${D}/lib
    ln -sf ./nexell/libdrm_nexell.so.1.0.0 libdrm_nexell.so
    ln -sf ./nexell/libdrm_nexell.so.1.0.0 libdrm_nexell.so.1

    install -m 0755 ${S}/tests/modetest/.libs/modetest ${D}${bindir}
}

INSANE_SKIP_${PN} = "compile-host-path dev-so debug-files"
FILES_${PN} = "${libdir} ${includedir} lib lib/nexell ${bindir}"
ALLOW_EMPTY_${PN} = "1"
PROVIDES = "libdrm-nx"