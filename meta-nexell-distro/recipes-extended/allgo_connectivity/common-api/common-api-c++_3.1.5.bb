SUMMARY = "CommonAPI"
SECTION = "libs"
LICENSE = "MPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MPL-2.0;md5=815ca599c9df247a0c7f619bab123dad"
PROVIDES = "commonapi3"
PR = "r0"

inherit autotools cmake lib_package pkgconfig

SRCREV = "77d9cdaf6acee03a1e360e0e1a3cd0b01a95b407"
SRC_URI = "file://common-api-runtime.tar.gz"
INHIBIT_PACKAGE_STRIP="1"
S = "${WORKDIR}/common-api-runtime/"
SRC_URI[md5sum] = "80a73aaeb2bdd2a133d76d4107534b53"
SRC_URI[sha256sum] = "1c42e31761d622806409ddd40dcc85e184c506019b9706a0546868ae91a6c96f"

EXTRA_OECMAKE += "-DCMAKE_INSTALL_PREFIX=/usr"

FILES_${PN}-dev += "${libdir}/cmake ${includedir}/CommonAPI-3.1/CommonAPI"
