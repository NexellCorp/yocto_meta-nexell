SUMMARY = "CommonAPI-DBus"
SECTION = "libs"
LICENSE = "MPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MPL-2.0;md5=815ca599c9df247a0c7f619bab123dad"
PR = "r0"
DEPENDS = "commonapi3 dbus"
REQUIRES = "commonapi3 dbus"

SRCREV = "3348a422ffc756b63de5890356383858a898e8b1"
SRC_URI = "file://common-api-dbus-runtime.tar.gz"
INHIBIT_PACKAGE_STRIP="1"
S = "${WORKDIR}/common-api-dbus-runtime"

SRC_URI[md5sum] = "10100f1dae15f7ee7a72193a058762f1"
SRC_URI[sha256sum] = "963283720767f81020e580aae69e4241e221b215b254f8dadfcb599291f97b28"

EXTRA_OECMAKE += "-DCMAKE_INSTALL_PREFIX=/usr "
CXXFLAGS := "${@oe.utils.str_filter_out('-fvisibility-inlines-hidden', '${CXXFLAGS}', d)}"

inherit cmake lib_package pkgconfig
FILES_${PN}-dev += "${libdir}/cmake ${includedir}/CommonAPI-3.1/CommonAPI/DBus ${includedir}/CommonAPI-3.1/pugixml"
