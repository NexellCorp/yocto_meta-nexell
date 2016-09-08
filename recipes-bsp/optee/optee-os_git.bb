DESCRIPTION = "OPTEE OS"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://LICENSE;md5=69663ab153298557a59c67a60a743e5b"

SRCREV = "523e3c9b41b36c6625761d5c0948999da630b82c"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_os;protocol=git;branch=yocto-dmc \
           file://0001-optee-os-compile-error-patch.patch"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p6818-avn-ref)"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

do_configure() {
    :
}

do_compile() {
    :
}
    
#addtask mypatch after do_unpack before do_patch
