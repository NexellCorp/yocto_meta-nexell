DESCRIPTION = "Example libraries injection"
LICENSE = "CLOSED"
SECTION = "libs"

LIC_FILES_CHKSUM = "file://README.md;md5=a42b1a2893cd41295427241303fe1f3d"
SRCREV = "72d7aad8bd82c1167817c63c409ff58bdb014c76"
SRC_URI = "git://github.com/kchhero/yocto_example_libs;protocol=git;branch=master"

SRC_URI += " \
    file://include \
    file://glesv2.pc \
    "

S = "${WORKDIR}/git"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p4418-avn-ref)"
PACKAGE_ARCH = "${MACHINE_ARCH}"

do_install () {
    install -d ${D}${libdir}/example-nexell-libs
    install -d ${D}${libdir}/example-nexell-modules
    install -d ${D}${libdir}/example-nexell-opts
    install -d ${D}${includedir}/example-nexell-inc
    
    install -m 0644 ${S}/lib/* ${D}${libdir}/example-nexell-libs
    install -m 0644 ${S}/modules/* ${D}${libdir}/example-nexell-modules
    install -m 0644 ${S}/opts/* ${D}${libdir}/example-nexell-opts
    
    install -m 0644 ${WORKDIR}/glesv2.pc ${D}${libdir}/example-nexell-opts/kkk-glesv2.pc
    install -m 0644 ${WORKDIR}/include/EGL/egl.h ${D}${includedir}/example-nexell-inc/kkk-egl.h
}

FILES_${PN} += "${libdir} ${includedir}"
