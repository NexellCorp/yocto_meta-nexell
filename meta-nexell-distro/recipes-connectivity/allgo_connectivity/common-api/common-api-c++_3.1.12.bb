SUMMARY = "CommonAPI"
SECTION = "libs"
LICENSE = "MPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MPL-2.0;md5=815ca599c9df247a0c7f619bab123dad"
PROVIDES = "commonapi3"
PR = "r0"

inherit autotools cmake lib_package pkgconfig

SRCREV = "5152d31a5607393103ef01f2bb034bd267064b81"
SRC_URI = "git://github.com/GENIVI/capicxx-core-runtime.git"
INHIBIT_PACKAGE_STRIP="1"
S = "${WORKDIR}/git"

EXTRA_OECMAKE += "-DCMAKE_INSTALL_PREFIX=/usr"

FILES_${PN}-dev += "${libdir}/cmake ${includedir}/CommonAPI-3.1/CommonAPI"
