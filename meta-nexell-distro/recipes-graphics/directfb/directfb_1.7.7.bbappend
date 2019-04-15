FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI += " \
	file://configure-tslib.patch \
	file://fix-tslib-1.15-ts_config.patch \
	file://combine-drmkms_system-vt-header.patch \
"

SRC_URI += " \
	file://directfbrc \
"

DFB_PACKAGE_CONFIGURE ??= ""
DFB_PACKAGE_CONFIGURE_DEPENDS ??= ""

PACKAGECONFIG[configure] = "${DFB_PACKAGE_CONFIGURE},,${DFB_PACKAGE_CONFIGURE_DEPENDS}"

do_install_append () {
	install -d ${D}${sysconfdir}
	install -m 644 ${WORKDIR}/directfbrc ${D}${sysconfdir}
}

FILES_${PN} += "${sysconfdir}"
