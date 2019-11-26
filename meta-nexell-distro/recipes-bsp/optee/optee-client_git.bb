DESCRIPTION = "OPTEE CLIENT"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://LICENSE;md5=69663ab153298557a59c67a60a743e5b"

PV ?= "1.0+git${SRCPV}"
SRCREV = "0b0d237779de6982451f6f5971b7106e459a2958"

SRC_URI = "git://review.gerrithub.io/NexellCorp/secure_optee_optee-client;protocol=https;branch=nexell"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p6818-avn-ref|s5p6818-kick-st|s5p6818-svt-ref)"

do_compile() {
    :
}
