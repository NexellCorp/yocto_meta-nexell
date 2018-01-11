SUMMARY = "Qt5 APP LIBS, NEXELL"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "qtbase qtmultimedia "
DEPENDS += "${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'qtwayland', '', d)}"
DEPENDS += "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.4', 'qtquickcontrols', 'qtquickcontrols2', d)}"

SRCREV = "478588c8c077c67a85b1a02b1b19817a20746f59"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/apps/QT/NxAudioPlayer;protocol=git;branch=master"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit autotools pkgconfig
require recipes-qt/qt5/qt5.inc

#HOST_SYS ==> arm-poky-linux-gnueabi
#oe_runconf is run at WORKDIR, so can't use oe_runconf. If want use oe_runconf, configure file has to located WORKDIR

#-----work flow-----
#QT app configure -> id3lib configure -> id3lib compile -> QT app compile

do_configure_append() {
    cd ${S}/id3lib-3.8
    ./configure --host=${HOST_SYS} --prefix=${S}/libid3/

    cd ${S}/id3lib-3.8/zlib
    ./configure --host=${HOST_SYS} --prefix=${S}/libid3/
}

do_nxAudioPlayer_preCompile() {
    cd ${S}/id3lib-3.8
    oe_runmake
    oe_runmake install

    cd ${S}/id3lib-3.8/zlib
    oe_runmake
    oe_runmake install
}

do_install() {
    install -d ${D}/podo/apps/NxAudioPlayer
    install -m 0644 ${S}/Package/* ${D}/podo/apps/NxAudioPlayer
    install ${B}/NxAudioPlayer/NxAudioPlayer ${D}/podo/apps/NxAudioPlayer
}

addtask nxAudioPlayer_preCompile before do_compile after do_configure

FILES_${PN} = "podo"
RDEPENDS_${PN} = "qtbase-plugins"