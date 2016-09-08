DESCRIPTION = "OPTEE TEST"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README.md;md5=89ee7495ae9d82329fb68c61ed15a7a1"

SRCREV = "b28d0dadd467b7bcf13cb89deaec0d7ea122480b"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_test;protocol=git;branch=yocto-dmc"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p6818-avn-ref)"

do_compile() {
    :
}
