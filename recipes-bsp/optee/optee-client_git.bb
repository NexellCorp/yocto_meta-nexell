DESCRIPTION = "OPTEE CLIENT"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://LICENSE;md5=69663ab153298557a59c67a60a743e5b"

SRCREV = "0b0d237779de6982451f6f5971b7106e459a2958"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_client;protocol=git;branch=yocto-hs-iot"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p6818-avn-ref)"

do_compile() {
    :
}