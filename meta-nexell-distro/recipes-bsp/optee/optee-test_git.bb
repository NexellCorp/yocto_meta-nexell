DESCRIPTION = "OPTEE TEST"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README.md;md5=89ee7495ae9d82329fb68c61ed15a7a1"

SRCREV = "75e4c82ea87442a2d291cb8b6971f30db9e9cef2"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_test;protocol=git;branch=nexell \
           file://0001-optee-test-compile-error-patch.patch "

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p6818-avn-ref|s5p6818-bitminer-ref|s5p6818-kick-st)"

do_compile() {
    :
}
