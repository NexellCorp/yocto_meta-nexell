DESCRIPTION = "l-loader for artik7"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://COPYING;md5=e8c1458438ead3c34974bc0be3a03ed6"

SRCREV = "8f662c7f65cd88be89876325d0cc7b2c6c60014e"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/l-loader;protocol=git;branch=artik"

S = "${WORKDIR}/git"

DEFAULT_PREFERENCE = "1"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor)"

do_mypatch() {
    git fetch http://59.13.55.140/gerrit/artik7/l-loader refs/changes/34/2134/1 && git cherry-pick FETCH_HEAD
}

do_compile() {
    :
}

addtask mypatch after do_unpack before do_patch