SUMMARY = "tslib re-calibration files"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"
  
SRC_URI = "file://ts_recalibrate.sh \
          "

do_install () {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/ts_recalibrate.sh ${D}${bindir}
}

FILES_${PN} += "${bindir}/ts_recalibrate.sh"
