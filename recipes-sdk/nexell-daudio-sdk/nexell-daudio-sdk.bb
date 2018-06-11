DESCRIPTION = "Nexell DAudio SDK"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "devel"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "nx-video-api nx-v4l2 nx-scaler ffmpeg qtwayland qtmultimedia"

inherit nexell-daudio-sdk-env

SRCREV = "${AUTOREV}"
SRC_URI = "git://git.nexell.co.kr/nexell/linux/sdk/displayaudio;protocol=git;branch=master"

PV = "1.0"
PR = "r0"

S = "${WORKDIR}/git"
SDK_RESULT = "${S}/result"

export OECORE_SDK_VERSION = "2.1.3"

D_SDK_INC = "-I./ -I../../include -I../../../include -I../../library/include -I../../../library/include -I${STAGING_INCDIR} -I${STAGING_INCDIR}/libxml2 -I${STAGING_INCDIR}/drm"
D_SDK_LIB = "-L./ -L../../library/lib -L../../../library/lib -L${STAGING_LIBDIR}"

CFLAGS_remove_arm = "-g"
CFLAGS_append = " -fPIC"
CFLAGS_prepend = "${D_SDK_INC}"
CXXFLAGS_remove_arm = "-g"
CXXFLAGS_append = " -fPIC"
CXXFLAGS_prepend = "${D_SDK_INC}"
LDFLAGS_prepend = "${D_SDK_LIB} "

do_install() {
	echo "Installing DAudio SDK..."

	install -d ${D}${libdir}
	install -d ${D}${bindir}
	install -d ${D}/nexell/daudio
	install -d ${D}/podo

	cp -aR ${SDK_RESULT}${libdir}/* ${D}${libdir}/
	chown -R root:root ${D}${libdir}/
	cp -aR ${SDK_RESULT}${bindir}/* ${D}${bindir}/
	chown -R root:root ${D}${bindir}/

	cp -aR ${SDK_RESULT}/nexell/daudio/* ${D}/nexell/daudio/
	chmod -R 755 ${D}/nexell/daudio/*
	chown -R root:root ${D}/nexell/daudio/*

	cp -aR ${SDK_RESULT}/podo/* ${D}/podo/
	chmod -R 755 ${D}/podo/*
	chown -R root:root ${D}/podo/*
}

FILES_${PN} = "${bindir} ${libdir} nexell/daudio podo"
RDEPENDS_${PN} = "nexell-drm-mali"

INSANE_SKIP_${PN} = "host-user-contaminated dev-deps dev-so staticdev"
INSANE_SKIP_${PN}-dev = "dev-elf textrel"

addtask do_generate_qt_config_file before do_compile
