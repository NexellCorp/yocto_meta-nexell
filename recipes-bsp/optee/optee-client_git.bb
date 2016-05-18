DESCRIPTION = "OPTEE CLIENT for artik7"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://LICENSE;md5=69663ab153298557a59c67a60a743e5b"

SRCREV = "0b0d237779de6982451f6f5971b7106e459a2958"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_client;protocol=git;branch=artik"

S = "${WORKDIR}/git"

do_compile() {
    echo -e "\033[44;33m[suker][optee_client]${WORKDIR}/git/\033[0m" >> ${TOPDIR}/mylog.txt
}