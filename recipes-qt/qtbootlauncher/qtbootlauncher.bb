SUMMARY = "Qt bootlauncher"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://launcher-cap.conf \
	file://launcher-press.conf \
	file://Makefile \
	file://qtbootlauncher.c \
	"

S = "${WORKDIR}"

PV = "NEXELL"
PR = "0.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
	oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="${CC}" LDFLAGS+="--sysroot=${STAGING_DIR_HOST}"
}

do_install() {
	install -d ${D}${systemd_unitdir}/system-generators
	install -d ${D}${sysconfdir}/qboot

	install -m 0755 ${S}/qtbootlauncher ${D}${systemd_unitdir}/system-generators/

	if [ "${NEXELL_TOUCH_CLASS}" = "CAPACITIVE" ]; then
		install -m 0755 ${S}/launcher-cap.conf ${D}${sysconfdir}/qboot/launcher.conf
	else
		install -m 0755 ${S}/launcher-press.conf ${D}${sysconfdir}/qboot/launcher.conf
	fi
}

FILES_${PN} = "${systemd_unitdir}/system-generators ${sysconfdir}/qboot"
INSANE_SKIP_${PN} = "installed-vs-shipped already-stripped"
