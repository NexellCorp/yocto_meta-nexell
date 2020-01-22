inherit linux-nexell-base

DESCRIPTION = "Nexell NxQuickRearCam process"
HOMEPAGE = "http://www.nexell.co.kr"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PV = "1.0"
PR = "r0"

SRCREV = "${AUTOREV}"

SRC_URI = "git://review.gerrithub.io/NexellCorp/linux_apps_NxQuickRearCam;protocol=https;branch=master"

DEPENDS = "nx-drm-allocator nx-v4l2 nx-scaler nx-video-api libdrm"

S = "${WORKDIR}/git"

inherit autotools pkgconfig gettext

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile() {
 	cd ${S}
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake \
		CROSS_COMPILE=${TARGET_PREFIX} \
		INCLUDES="-I${STAGING_INCDIR}	\
				-I${STAGING_INCDIR}/nexell \
				-I${STAGING_INCDIR}/libdrm \
				-I${STAGING_INCDIR}/drm" \ 
				LDFLAGS="-L${STAGING_LIBDIR}" CC="$CC"
}

do_install() {
	install -d ${D}${base_sbindir}
	install -m 0755 ${S}/apps/NxQuickRearCam/NxQuickRearCam ${D}${base_sbindir}/NxQuickRearCam
}

FILES_${PN} = "${base_sbindir}"

INSANE_SKIP_${PN} = "already-stripped"
