SUMMARY = "Qt5 APP LIBS, NEXELL"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "qtbase qtquickcontrols2 qtmultimedia qtwebkit qtwayland nx-video-api nx-v4l2 libdrm "

SRCREV = "b9a4df170705b85082a487dc54c5b0d5f969272b"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/apps/QT/NxQuickRearCam;protocol=git;branch=master"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit autotools pkgconfig
require recipes-qt/qt5/qt5.inc

do_librearcam_preCompile() {
    cd ${S}/librearcam

    oe_runmake  LIBPATH="${STAGING_LIBDIR}" CROSSNAME="${TARGET_PREFIX}" INCPATH="${STAGING_INCDIR}" LDFLAGS=" --sysroot=${STAGING_DIR_HOST}" \
                CPPFLAGS=" --sysroot=${STAGING_DIR_HOST}"
}

do_compile_prepend() {
    mkdir -p ${B}/librearcam/lib
    cp -a ${S}/librearcam/lib/*.so ${B}/librearcam/lib
}

do_install() {
    install -d ${D}/podo/apps/NxQuickRearCam
    install -d ${D}${libdir}
    install -m 0644 ${S}/Package/* ${D}/podo/apps/NxQuickRearCam
    install ${B}/NxQuickRearCam/NxQuickRearCam ${D}/podo/apps/NxQuickRearCam
	install -m -644 ${B}/librearcam/lib/*.so ${D}${libdir}
}

addtask librearcam_preCompile before do_compile after do_configure

FILES_${PN} = "podo ${libdir}"
RDEPENDS_${PN} = "qtbase-plugins"
INSANE_SKIP_${PN}-dev = "dev-elf"
