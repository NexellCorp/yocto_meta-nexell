SUMMARY = "CommonAPI-DBus"
SECTION = "libs"
LICENSE = "MPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MPL-2.0;md5=815ca599c9df247a0c7f619bab123dad"
PR = "r0"
DEPENDS = "commonapi3 dbus"
REQUIRES = "commonapi3 dbus"

SRCREV = "5f694be78a2520cbdea31e8ba82cb579ebf893f9"
SRC_URI = "git://github.com/GENIVI/capicxx-dbus-runtime.git"
S = "${WORKDIR}/git"

EXTRA_OECMAKE += "-DCMAKE_INSTALL_PREFIX=/usr "
CXXFLAGS := "${@oe.utils.str_filter_out('-fvisibility-inlines-hidden', '${CXXFLAGS}', d)}"

inherit cmake lib_package pkgconfig
FILES_${PN}-dev += "${libdir}/cmake ${includedir}/CommonAPI-3.1/CommonAPI/DBus ${includedir}/CommonAPI-3.1/pugixml"
