LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://README;md5=c48cff868868eb40f73e510f7bae7710"
SRCREV = "3ddb2d32fb91ae40d5bc5b8a35acdc762ac6b11c"
SRC_URI = "git://git.nexell.co.kr/nexell/bl1/bl1-s5p4418;protocol=git;branch=nexell"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"