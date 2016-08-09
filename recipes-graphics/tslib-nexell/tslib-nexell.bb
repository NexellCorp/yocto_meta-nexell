SUMMARY = "tslib re-calibration files"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

COMPATIBLE_MACHINE="(s5p4418-navi-ref|s5p4418-avn-ref|s5p6818-artik710-raptor|s5p6818-avn-ref)"
PACKAGE_ARCH = "${MACHINE_ARCH}"
  
SRC_URI = "file://ts_recalibrate.sh \
          "

do_install () {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/ts_recalibrate.sh ${D}${bindir}
}

FILES_${PN} += "${bindir}/ts_recalibrate.sh"
