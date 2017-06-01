SUMMARY = "Nexell smart voice test application"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRCREV = "85ae05be395fdcfafe1ad44a0f431b246755ff7e"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/apps/smart_voice_apps;protocol=git"

PV = "NEXELL"
PR = "0.1"
S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
    cd ${S}/src
    oe_runmake clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} LDFLAGS+=" -mfloat-abi=hard --sysroot=${STAGING_DIR_HOST} " CC="${CC}" \
               INCLUDE="-I${STAGING_INCDIR} \
                        -I${S}/src \
                        -I${S}/lib \
                        -I${STAGING_INCDIR}/c++/5.3.0 \
                        -I${STAGING_INCDIR}/c++/5.3.0/${HOST_SYS}/bits \
                        -I${STAGING_INCDIR}/c++/5.3.0/${HOST_SYS}"              
}

do_install() {
    install -d ${D}${bindir}

    install -m 0755 ${S}/src/smart_voice ${D}${bindir}
}

FILES_${PN} = "${bindir} "