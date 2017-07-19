SUMMARY = "Nexell smart voice test application"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRCREV = "1252cd712c301ad5fb256ac8a7732b1f52334f1a"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/apps/smart_voice_apps;protocol=git"

PV = "NEXELL"
PR = "0.1"
S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
    cd ${S}/libresample
    oe_runmake clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} \
               CFLAGS="-Wall -mfloat-abi=hard -O3 -fomit-frame-pointer -march=armv7-a -mfpu=neon -mcpu=cortex-a9 --sysroot=${STAGING_DIR_HOST}" \
               CXXFLAGS="-march=armv7-a -mfpu=neon -mcpu=cortex-a9 --sysroot=${STAGING_DIR_HOST}" \
               INC_DIR="${S}/lib" LIB_DIR="${S}/lib"

    cd ${S}/src
    oe_runmake clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} \
               CFLAGS+="-mfloat-abi=hard --sysroot=${STAGING_DIR_HOST} -I${S}/lib -I${S}/lib/hf" \
               LDFLAGS+="--sysroot=${STAGING_DIR_HOST}" \
               INC_DIR="${S}/lib/hf" LIB_DIR="${S}/lib/hf" INCLUDE="-I${STAGING_INCDIR}"
}

do_install() {
    install -d ${D}${bindir}

    install -m 0755 ${S}/src/smart_voice ${D}${bindir}
}

FILES_${PN} = "${bindir} "