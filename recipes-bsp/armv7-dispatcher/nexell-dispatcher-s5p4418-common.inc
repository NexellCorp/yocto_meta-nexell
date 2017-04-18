LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://README;md5=c48cff868868eb40f73e510f7bae7710"
SRCREV = "705db5400f67fc14ae6d742582c3f9b53a8fff1f"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/armv7-dispatcher;protocol=git;branch=nexell"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"