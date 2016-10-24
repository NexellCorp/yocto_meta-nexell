DESCRIPTION = "l-loader for s5p6818"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://COPYING;md5=e8c1458438ead3c34974bc0be3a03ed6"

SRCREV = "884bd184f988f442b98677afbcd80f3aebbef00d"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/l-loader;protocol=git;branch=yocto-hs-iot"

S = "${WORKDIR}/git"

DEFAULT_PREFERENCE = "1"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p6818-avn-ref)"

do_compile() {
    :
}

#addtask mypatch after do_unpack before do_patch