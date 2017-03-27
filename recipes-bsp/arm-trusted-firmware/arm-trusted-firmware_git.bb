DESCRIPTION = "ATF for artik7"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://license.md;md5=829bdeb34c1d9044f393d5a16c068371"

SRCREV = "a2684acd0c492611a294a7cc9997eb81d9288e01"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/arm-trusted-firmware;protocol=git;branch=nexell \
          "

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p6818-avn-ref)"

do_compile() {
    :
}

#addtask mypatch after do_unpack before do_patch