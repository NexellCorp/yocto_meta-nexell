DESCRIPTION = "OPTEE OS for artik7"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://LICENSE;md5=69663ab153298557a59c67a60a743e5b"

SRCREV = "cd767e4006afcd8275122e1a5b2562827d4078ee"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_os;protocol=git;branch=artik"

S = "${WORKDIR}/git"

do_compile() {
    echo -e "\033[44;33m[suker][optee_os]${WORKDIR}/git/\033[0m" >> ${TOPDIR}/mylog.txt
}