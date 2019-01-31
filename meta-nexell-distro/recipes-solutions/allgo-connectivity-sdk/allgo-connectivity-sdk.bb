DESCRIPTION = "Nexell allgo connectivity SDK"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "devel"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://sdk \
"

DEPENDS = " \
	common-api-c++-dbus \
	common-api-c++ \
	qtbase-native \
	qtmultimedia \
	json-glib \
	protobuf-c \
	icu \
"

RPROVIDES_${PN} = " \
	nexell-carconn-sdk-dev \
	libheromanager.so \
	libQt5WaylandCompositor.so.5 \
"

RDEPENDS_${PN} = " \
	bash \
	nexell-daudio-sdk \
"

inherit nexell-sdk-qt-env

PV = "1.0.0"
PR = "r0"

S = "${WORKDIR}/sdk"
SDK_RESULT = "${S}/result"

export OECORE_SDK_VERSION = "${SDK_VERSION}"

export alternate_out_path = "${NX_CARPLAY_ALTERNATE_OUT_PATH}"
export default_out_path = "${NX_CARPLAY_DEFAULT_OUT_PATH}"
export media_out_path = "${NX_CARPLAY_MEDIA_OUT_PATH}"
export telephony_in_path = "${NX_CARPLAY_TELEPHONY_IN_PATH}"
export telephony_out_path = "${NX_CARPLAY_TELEPHONY_OUT_PATH}"
export siri_in_path = "${NX_CARPLAY_SIRI_IN_PATH}"
export siri_out_path = "${NX_CARPLAY_SIRI_OUT_PATH}"
export alert_out_path = "${NX_CARPLAY_ALERT_OUT_PATH}"

do_install() {
	echo "Installing allgo connectivity SDK..."

	install -d ${D}${sysconfdir}
	install -d ${D}${libdir}
	install -d ${D}${bindir}
	install -d ${D}/home/root
	install -d ${D}/nexell/daudio

	cp -apR ${SDK_RESULT}${sysconfdir}/* ${D}${sysconfdir}/
	chown -R root:root ${D}${sysconfdir}/
	cp -apR ${SDK_RESULT}${libdir}/* ${D}${libdir}/
	chown -R root:root ${D}${libdir}/
	cp -apR ${SDK_RESULT}${bindir}/* ${D}${bindir}/
	chown -R root:root ${D}${bindir}/
	cp -apR ${SDK_RESULT}/home/root/* ${D}/home/root/
	chmod -R 755 ${D}/home/root/*
	chown -R root:root ${D}/home/root/*
	cp -apR ${SDK_RESULT}/nexell/daudio/* ${D}/nexell/daudio/
	chmod -R 755 ${D}/nexell/daudio/*
	chown -R root:root ${D}/nexell/daudio/*
}

FILES_${PN} = "${sysconfdir} ${bindir} ${libdir} home/root nexell/daudio"
FILES_${PN}-dev = "${includedir}"

INSANE_SKIP_${PN} = "dev-deps dev-so textrel already-stripped"
INSANE_SKIP_${PN}-dev = "dev-elf textrel"

addtask do_generate_qt_config_file before do_compile
