SUMMARY = "CRIU"
DESCRIPTION = "Checkpoint/Restore In Userspace, or CRIU, is a software tool for \
Linux operating system. Using this tool, you can freeze a running application \
(or part of it) and checkpoint it to a hard drive as a collection of files. \
You can then use the files to restore and run the application from the point \
it was frozen at. The distinctive feature of the CRIU project is that it is \
mainly implemented in user space"
HOMEPAGE = "http://criu.org"
SECTION = "console/tools"

EXCLUDE_FROM_WORLD = "1"

LICENSE = "GFDL-1.3"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GFDL-1.3;md5=1083add59b39991c748ea70a92166959"

SRC_URI = "file://criu-3.12.tar.gz"

COMPATIBLE_HOST = "(x86_64|arm|aarch64).*-linux"

DEPENDS += "libnl libcap protobuf-c-native protobuf-c util-linux-native libbsd libnet"
RDEPENDS_${PN} = "bash"
RDEPENDS_${PN} += "${PYTHON_PN}-argparse"

S = "${WORKDIR}/criu-3.12"

#
# CRIU just can be built on ARMv7 and ARMv6, so the Makefile check
# if the ARCH is ARMv7 or ARMv6.
# ARM BSPs need set CRIU_BUILD_ARCH variable for building CRIU.
#
EXTRA_OEMAKE_arm += "ARCH=arm UNAME-M=${CRIU_BUILD_ARCH} WERROR=0"
EXTRA_OEMAKE_x86-64 += "ARCH=x86 WERROR=0"
EXTRA_OEMAKE_aarch64 += "ARCH=arm64 WERROR=0"

EXTRA_OEMAKE_append += "SBINDIR=${sbindir} LIBDIR=${libdir} INCLUDEDIR=${includedir} PIEGEN=no"
EXTRA_OEMAKE_append += "LOGROTATEDIR=${sysconfdir} SYSTEMDUNITDIR=${systemd_unitdir}"

CFLAGS += "-D__USE_GNU -D_GNU_SOURCE " 

CFLAGS += " -I${STAGING_INCDIR} -I${STAGING_INCDIR}/libnl3"
CFLAGS_arm += "-D__WORDSIZE"

# overide LDFLAGS to allow criu to build without: "x86_64-poky-linux-ld: unrecognized option '-Wl,-O1'"
export LDFLAGS=""
export C_INCLUDE_PATH="${STAGING_INCDIR}/libnl3"

export BUILD_SYS
export HOST_SYS
export HOSTCFLAGS = "${BUILD_CFLAGS}"

inherit setuptools

PACKAGECONFIG ??= ""
PACKAGECONFIG[selinux] = ",,libselinux"

CLEANBROKEN = "1"

do_compile_prepend() {
    rm -rf ${S}/images/google/protobuf/descriptor.proto
    ln -s  ${PKG_CONFIG_SYSROOT_DIR}/usr/include/google/protobuf/descriptor.proto ${S}/images/google/protobuf/descriptor.proto
}

do_compile () {
    oe_runmake FULL_PYTHON=${PYTHON} PYTHON=python2
}

do_install () {
    export INSTALL_LIB="${libdir}/${PYTHON_DIR}/site-packages"
    oe_runmake PREFIX=${exec_prefix} LIBDIR=${libdir} DESTDIR="${D}" FULL_PYTHON=${PYTHON} PYTHON=python2 install
}

FILES_${PN} += "${systemd_unitdir}/ \
            ${libdir}/python2.7/site-packages/ \
            ${libdir}/pycriu/ \
            ${libdir}/crit-0.0.1-py2.7.egg-info \
            "

FILES_${PN}-staticdev += " \
            ${libexecdir}/compel/std.lib.a \
            ${libexecdir}/compel/fds.lib.a \
            "
