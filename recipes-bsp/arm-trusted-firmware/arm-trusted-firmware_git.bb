DESCRIPTION = "ATF for artik7"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://license.md;md5=829bdeb34c1d9044f393d5a16c068371"

SRCREV = "c4061fcdfff081f40659241fccb9f504dc8b4960"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/arm-trusted-firmware;protocol=git;branch=artik"

S = "${WORKDIR}/git"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "s5p6818-artik710-raptor"

do_mypatch() { 
    git fetch http://59.13.55.140/gerrit/artik7/arm-trusted-firmware refs/changes/70/2070/3 && git cherry-pick FETCH_HEAD
}

do_compile() {
    :
}

addtask mypatch after do_unpack before do_patch