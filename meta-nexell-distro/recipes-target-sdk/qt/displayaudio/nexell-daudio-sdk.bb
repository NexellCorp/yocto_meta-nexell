DESCRIPTION = "Nexell displayaudio SDK"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "devel"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://sdk \
"

DEPENDS = " \
	nx-drm-allocator \
	nx-video-api \
	nx-scaler \
	nx-v4l2 \
	ffmpeg \
	id3lib \
	libid3tag \
	libjpeg-turbo \
    icu \
	qtbase-native \
	qtmultimedia \
"

RPROVIDES_${PN} = " \
	nexell-daudio-sdk-dev \
	libnxbt.so \
	libnxconfig.so \
	libnxdaudioutils.so \
	libnxfilter.so \
	libnxfilterhelper.so \
	libnxkeyreceiver.so \
	libnxmpmanager.so \
	libnxrearcam.so \
"

inherit nexell-daudio-sdk-env

PV = "1.0"
PR = "r0"

S = "${WORKDIR}/sdk"
SDK_RESULT = "${S}/result"

export OECORE_SDK_VERSION = "2.5.1"

D_SDK_INC =  " -I./"
D_SDK_INC += " -I../../include -I../../../include -I../../library/include -I../../../library/include"
D_SDK_INC += " -I../../prebuilt/include -I../../../prebuilt/include -I../../prebuilt/include -I../../../library/include"
D_SDK_INC += " -I${STAGING_INCDIR} -I${STAGING_INCDIR}/libxml2 -I${STAGING_INCDIR}/drm"

D_SDK_LIB =  " -L./"
D_SDK_LIB += " -L../../library/lib -L../../../library/lib"
D_SDK_LIB += " -L../../prebuilt/library/lib -L../../../prebuilt/library/lib"
D_SDK_LIB += " -L${STAGING_LIBDIR}"

CFLAGS_remove_arm = "-g"
CFLAGS_append = " -fPIC"
CFLAGS_prepend = "${D_SDK_INC}"
CXXFLAGS_remove_arm = "-g"
CXXFLAGS_append = " -fPIC"
CXXFLAGS_prepend = "${D_SDK_INC}"
LDFLAGS_prepend = "${D_SDK_LIB} "

do_install() {
	echo "Installing displayaudio SDK..."

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

INSANE_SKIP_${PN} = "dev-deps dev-so textrel"
INSANE_SKIP_${PN}-dev = "dev-elf textrel"

addtask do_generate_qt_config_file before do_compile
