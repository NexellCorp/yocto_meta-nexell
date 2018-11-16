SUMMARY = "tslib re-calibration files"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = " \
	file://ts_recalibrate.sh \
	file://calibration \
"

TS_POINTERCAL = "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-daudio-ref nexell-navi-ref', 'pointercal', 'not_supported', d)}"

do_install () {
	install -d ${D}${bindir}
	install -d ${D}${sysconfdir}

    install -m 0755 ${WORKDIR}/ts_recalibrate.sh ${D}${bindir}/

	if [ ${TS_POINTERCAL} != "not_supported" ]; then
	    install -m 0644 ${WORKDIR}/calibration/${TS_POINTERCAL} ${D}${sysconfdir}/pointercal
	else
		echo "Can't find supporetd calibration file."
	fi
}

FILES_${PN} += "${bindir} ${sysconfdir}"
