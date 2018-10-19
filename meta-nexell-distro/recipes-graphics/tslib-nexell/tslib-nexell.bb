SUMMARY = "tslib re-calibration files"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"

SRC_URI = "file://ts_recalibrate.sh \
           file://calibration \
          "

TS_POINTERCAL = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-daudio-ref', 'pointercal-daudio', 'pointercal', d)}"

do_install () {
    install -d ${D}${bindir}
    install -d ${D}/etc

    install -m 0755 ${WORKDIR}/ts_recalibrate.sh ${D}${bindir}
    install -m 0644 ${WORKDIR}/calibration/${TS_POINTERCAL} ${D}/etc/pointercal
}

FILES_${PN} += "${bindir}/ts_recalibrate.sh etc/pointercal"
