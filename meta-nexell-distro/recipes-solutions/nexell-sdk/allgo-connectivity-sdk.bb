DESCRIPTION = "Nexell allgo connectivity SDK"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "devel"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = " \
	libxml2 \
	libusb1 \
	nx-uds \
	nx-config \
	common-api-c++-dbus \
	common-api-c++ \
	qtbase-native \
	qtmultimedia \
	json-glib \
	protobuf-c \
	icu \
	openssl \
	gstreamer1.0-plugins-base \
"

RDEPENDS_${PN} = " \
	bash \
"

inherit ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.6', 'nexell-sdk-qt5.6.x-env', \
           bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.8', 'nexell-sdk-qt5.8.x-env', '', d), d)}

PV = "1.0.0"
PR = "r0"

S = "${WORKDIR}/sdk"
SDK_RESULT = "${S}/result"

export OECORE_SDK_VERSION = "${SDK_VERSION}"

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
