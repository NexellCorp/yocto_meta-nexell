DESCRIPTION = "OPTEE OS for artik7"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://LICENSE;md5=69663ab153298557a59c67a60a743e5b"

SRCREV = "cd767e4006afcd8275122e1a5b2562827d4078ee"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_os;protocol=git;branch=artik \
           file://0001-For-Yocto-temporary-commit.patch"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "(artik710-raptor|artik530-raptor)"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

do_mypatch() {
    git fetch http://59.13.55.140/gerrit/artik7/optee_os refs/changes/76/2076/1 && git cherry-pick FETCH_HEAD;
    git fetch http://59.13.55.140/gerrit/artik7/optee_os refs/changes/35/2135/1 && git cherry-pick FETCH_HEAD
}

do_configure() {
    :
}

do_compile() {
    :
}
    
addtask mypatch after do_unpack before do_patch