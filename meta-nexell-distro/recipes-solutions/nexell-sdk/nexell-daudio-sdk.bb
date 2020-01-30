DESCRIPTION = "Nexell daudio SDK"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "devel"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	git://github.com/NexellCorp/linux_sdk_displayaudio.git \
"

DEPENDS = " \
	libxml2 \
	nx-drm-allocator \
	nx-video-api \
	nx-scaler \
	nx-v4l2 \
	nx-uds \
	nx-config \
	ffmpeg \
	id3lib \
	libid3tag \
	libjpeg-turbo \
	taglib \
	icu \
	qtbase-native \
	qtmultimedia \
	gstreamer1.0-plugins-base \
"

inherit ${@bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.6', 'nexell-sdk-qt5.6.x-env', \
		   bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.8', 'nexell-sdk-qt5.8.x-env', \
           bb.utils.contains('DISTRO_FEATURES', 'nexell-qt5.9', 'nexell-sdk-qt5.9.x-env', '', d), d), d) }

PV = "1.4.0"
PR = "r0"

S = "${WORKDIR}/sdk"
SDK_RESULT = "${S}/result"

export OECORE_SDK_VERSION = "${SDK_VERSION}"
export OECORE_SOCNAME = "${NEXELL_BOARD_SOCNAME}"
export TARGET_MACHINE = "${MACHINE}"
export NX_DAUDIO_ENABLE_BT = "${ENABLE_BT}"
export NX_DAUDIO_ENABLE_CAM = "${ENABLE_CAM}"

D_SDK_INC += " -I${STAGING_INCDIR}/libxml2 -I${STAGING_INCDIR}/drm"

CFLAGS_prepend = "${D_SDK_INC}"
CXXFLAGS_prepend = "${D_SDK_INC}"

do_install() {
	echo "Installing daudio SDK..."

	install -d ${D}${libdir}
	install -d ${D}${bindir}
	install -d ${D}/nexell/daudio

	cp -apR ${SDK_RESULT}${libdir}/* ${D}${libdir}/
	chown -R root:root ${D}${libdir}/
	cp -apR ${SDK_RESULT}${bindir}/* ${D}${bindir}/
	chown -R root:root ${D}${bindir}/
	cp -apR ${SDK_RESULT}/nexell/daudio/* ${D}/nexell/daudio/
	chmod -R 755 ${D}/nexell/daudio/*
	chown -R root:root ${D}/nexell/daudio/*
}

FILES_${PN} = "${bindir} ${libdir} nexell/daudio"
FILES_${PN}-dev = "${includedir}"

INSANE_SKIP_${PN} = "dev-deps dev-so textrel already-stripped"
INSANE_SKIP_${PN}-dev = "dev-elf textrel"
