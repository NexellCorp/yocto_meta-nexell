DESCRIPTION = "OPTEE LINUXDRIVER for artik7"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRCREV = "5fcce5d5800a60957141f1d963edfd199480bfcb"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_linuxdriver;protocol=git;branch=artik"

S = "${WORKDIR}/git"

do_compile() {
    echo -e "\033[44;33m[suker][optee_linuxdriver]${WORKDIR}/git/\033[0m" >> ${TOPDIR}/mylog.txt
}