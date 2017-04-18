LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://Makefile;md5=c0a1617b8f4b81ee8c974952de5c1bee"
SRCREV = "cea03e08e91ad8071a59bd96b7962fa0b5d809b7"
SRC_URI = "git://git.nexell.co.kr/nexell/bl2/bl2-s5p4418;protocol=git;branch=nexell"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"