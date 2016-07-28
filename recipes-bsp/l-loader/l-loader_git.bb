DESCRIPTION = "l-loader for artik7"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://COPYING;md5=e8c1458438ead3c34974bc0be3a03ed6"

SRCREV = "65e8ffa03db8eb9955ba3e986ed6d6a31459ea52"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/l-loader;protocol=git;branch=artik"

S = "${WORKDIR}/git"

DEFAULT_PREFERENCE = "1"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor)"

do_compile() {
    :
}

#addtask mypatch after do_unpack before do_patch