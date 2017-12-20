SUMMARY = "Nexell smart voice test application"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRCREV = "${AUTOREV}"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/apps/smart_voice_apps;protocol=git"

PV = "NEXELL"
PR = "0.1"
S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
    cd ${S}
    oe_runmake clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} PKG_LDFLAGS+=" --sysroot=${STAGING_DIR_HOST} " CC="${CC}" \
               INCLUDE="-I${STAGING_INCDIR} \
                        -I${STAGING_INCDIR}/c++/5.3.0 \
                        -I${STAGING_INCDIR}/c++/5.3.0/${HOST_SYS}/bits \
                        -I${STAGING_INCDIR}/c++/5.3.0/${HOST_SYS}"              
}

do_install() {
    install -d ${D}${bindir}
    mkdir -p ${D}/usr/share/nexell/smart-voice

    install -m 0755 ${S}/smart-voice/smart_voice ${D}${bindir}
    install -m 0755 ${S}/smart-voice/samples/DingDong.wav ${D}/usr/share/nexell/smart-voice/
}

INSANE_SKIP_${PN} = "already-stripped"
FILES_${PN} = "${bindir} /usr/share/nexell/smart-voice/"
