DESCRIPTION = "Graphics libraries for S5P6818"
LICENSE = "Proprietary"
SECTION = "libs"

PROVIDES += "virtual/egl virtual/libgles1 virtual/libgles2"

S = "${WORKDIR}"

MALI_URI = "http://malideveloper.arm.com/downloads/drivers/binary"

RDEPENDS_${PN} += "kernel-modules"

INSANE_SKIP_${PN} = "ldflags"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"

USE_X11 = "${@base_contains("DISTRO_FEATURES", "x11", "yes", "no", d)}"
USE_DFB = "${@base_contains("DISTRO_FEATURES", "directfb", "yes", "no", d)}"
USE_WL = "${@base_contains("DISTRO_FEATURES", "wayland", "yes", "no", d)}"
USE_FB = "${@base_contains("DISTRO_FEATURES", "linuxfb", "yes", "no", d)}"

COMPATIBLE_MACHINE = "artik710-raptor"
PACKAGE_ARCH = "${MACHINE_ARCH}"

python __anonymous() {
    packages = d.getVar('PACKAGES', True).split()
    for p in packages:
        d.appendVar("INSANE_SKIP_%s" % p, " ldflags")

    for p in (("libegl", "libegl1"), ("libgl", "libgl1"),
    	("libgles1", "libglesv1cm1"), ("libgles2", "libglesv2-2"),
	("libgles3",)):
	fullp = p[0]
	pkgs = " ".join(p)
	d.setVar("DEBIAN_NOAUTONAME_" + fullp, "1")
	d.appendVar("RREPLACES_" + fullp, pkgs)
	d.appendVar("RPROVIDES_" + fullp, pkgs)
	d.appendVar("RCONFLICTS_" + fullp, pkgs)

	# For -dev, the first element is both the Debian and original name
	fullp += "-dev"
	pkgs = p[0] + "-dev"
	d.setVar("DEBIAN_NOAUTONAME_" + fullp, "1")
	d.appendVar("RREPLACES_" + fullp, pkgs)
	d.appendVar("RPROVIDES_" + fullp, pkgs)
	d.appendVar("RCONFLICTS_" + fullp, pkgs)
}


do_install () {
    install -d ${D}${libdir}
    install -d ${D}${includedir}
    install -d ${D}${bindir}

    cp -P ${S}/lib/*.so* ${D}${libdir}
    cp -r ${S}/include/* ${D}${includedir}
    
    install -d ${D}${libdir}/pkgconfig

    # The preference order, based in DISTRO_FEATURES, is x11, wayland, directfb and fb
    install -m 0644 ${WORKDIR}/egl.pc ${D}${libdir}/pkgconfig/egl.pc
    install -m 0644 ${WORKDIR}/glesv1_cm.pc ${D}${libdir}/pkgconfig/glesv1_cm.pc
    install -m 0644 ${WORKDIR}/glesv2.pc ${D}${libdir}/pkgconfig/glesv2.pc
    install -m 0644 ${WORKDIR}/vg.pc ${D}${libdir}/pkgconfig/vg.pc
    install -m 0644 ${WORKDIR}/bin/* ${D}${bindir}
    install -m 0644 ${WORKDIR}/module/*.ko ${D}${bindir}
    
    install -d ${D}/${sysconfdir}

#    install -m 644 ${WORKDIR}/directfbrc ${D}/${sysconfdir}/directfbrc

#    cp -r ${S}/usr/lib/directfb-1.6-0 ${D}${libdir}

#    find ${D}${libdir} -type f -exec chmod 644 {} \;
#    find ${D}${includedir} -type f -exec chmod 644 {} \;

    cp -pPR ${S}/lib/libMali.so ${D}/${libdir}/libEGL.so
    cp -pPR ${S}/lib/libMali.so ${D}/${libdir}/libGLESv2.so
    cp -pPR ${S}/lib/libdrm.so.2.4.0 ${D}/${libdir}/
}

#PACKAGES = "${PN}"
PACKAGES = "${PN} \
             libEGL.so libGLESv2.so \
            "

FILES_${PN} += "${sysconfdir}/MALI_MANIFEST \
	        ${libdir} ${bindir} ${includedir} "
		
FILES_${PN}-dev = "${libdir}/pkgconfig ${includedir}"

REALSOLIBS := "${SOLIBS}"
SOLIBS = "${SOLIBSDEV}"

INSANE_SKIP_libegl += "dev-so"
FILES_libegl = "${libdir}/libEGL${REALSOLIBS} ${libdir}/libEGL${SOLIBSDEV} "
FILES_libegl-dev = "${includedir}/EGL ${includedir}/KHR ${libdir}/pkgconfig/egl.pc"
FILES_libegl-dbg = "${libdir}/.debug/libEGL${SOLIBS}"

# libEGL needs to open libGLESv1.so
INSANE_SKIP_libgles += "dev-so"
FILES_libgles = "${libdir}/libGLESv1*${REALSOLIBS} ${libdir}/libGLESv1*${SOLIBS} ${libdir}/libGLES_*${SOLIBS}"
FILES_libgles-dev = "${includedir}/GLES ${libdir}/libGLESv1*${SOLIBS} ${libdir}/libGLES_*${SOLIBSDEV} ${libdir}/pkgconfig/glesv1_cm.pc"
FILES_libgles-dbg = "${libdir}/.debug/libGLESv1*${SOLIBS} ${libdir}/.debug/libGLES_*${SOLIBS}"

# libEGL needs to open libGLESv2.so
INSANE_SKIP_libgles2 += "dev-so"
FILES_libgles2 = "${libdir}/libGLESv2${REALSOLIBS} ${libdir}/libGLESv2${SOLIBS}"
FILES_libgles2-dev = "${includedir}/GLES2 ${libdir}/libGLESv2${SOLIBSDEV} ${libdir}/pkgconfig/glesv2.pc"
FILES_libgles2-dbg = "${libdir}/.debug/libGLESv2${SOLIBS}"

#FILES_libegl-staticdev = "${libdir}/lib*.a"

#RREPLACES_${PN} = "libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libopencl"
#RPROVIDES_${PN} = "libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libopencl"
#RCONFLICTS_${PN} = "libegl libegl1 libgles1 libglesv1-cm1 libgles2 libglesv2-2 libopencl"