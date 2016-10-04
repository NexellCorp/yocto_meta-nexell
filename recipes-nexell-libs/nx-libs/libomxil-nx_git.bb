DESCRIPTION = "nexell libomxil-nx"

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://Makefile;md5=d8309eef6670499b1b0924f0f648b17c"

SRCREV = "04465d3d574298c847c83c31f42f6811b3f9eacc"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/library/libomxil-nx;protocol=git;branch=nexell"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

#DEPENDS = "nx-v4l2 nx-drm-allocator nx-gst-meta gstreamer1.0 gstreamer1.0-plugins-base glib-2.0"

do_compile() {
    export NATIVE_BUILD=yes
    oe_runmake clean
    oe_runmake "INSTALL_DIR=${STAGING_LIBDIR}"
}

do_install() {
    install -d ${D}${includedir}
    install -d ${D}${includedir}/linux
#    install -d ${D}/lib
    install -m 0644 ${S}/include/linux/*.h ${D}${includedir}/linux
#    install -m 0644 ${S}/codec/libnx_video_api/nx_video_api.h ${D}${includedir}
#    install -m 0644 ${S}/codec/libnx_video_api/nx_video_alloc/nx_video_alloc.h ${D}${includedir}
#    install -m 0644 ${S}/lib/libnx_video_api.a ${D}/lib
}

INSANE_SKIP_${PN} = "compile-host-path dev-so debug-files"
FILES_${PN} = "${includedir}"
ALLOW_EMPTY_${PN} = "1"