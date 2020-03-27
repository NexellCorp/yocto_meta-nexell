FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI += " \
	file://configure-tslib.patch \
	file://fix-tslib-1.15-ts_config.patch \
	file://combine-drmkms_system-vt-header.patch \
	file://fix-build-error-use-PTHREAD_MUTEX_RECURSIVE.patch \
	file://support_argb_blend_smooth_scale.patch \
	file://drmkms_surface_pool_priority.patch \
	file://static_link_kms_driver.patch \
	file://drmkms_dumb.c \
	file://libdrm_macros.h \
"

SRC_URI += " \
	file://directfbrc \
"

# "<true opt>,<false opt>,<depend package>"
DFB_PACKAGE_CONFIGURE ??= ",,"

PACKAGECONFIG[configure] = "${DFB_PACKAGE_CONFIGURE}"
PACKAGECONFIG[debug] = "--enable-debug-support --enable-debug,,"
PACKAGECONFIG[trace] = "--enable-trace,,"

do_patch[postfuncs] += "do_patch_files"
do_patch_files() {
	install -m 644 ${WORKDIR}/drmkms_dumb.c ${S}/systems/drmkms/
	install -m 644 ${WORKDIR}/libdrm_macros.h ${S}/systems/drmkms/
}

do_install_append () {
	install -d ${D}${sysconfdir}
	install -m 644 ${WORKDIR}/directfbrc ${D}${sysconfdir}

	if [ ! -z "${DIRECTFB_TSLIB_DEVICES}" ]; then
		echo "tslib-devices = ${DIRECTFB_TSLIB_DEVICES}" \
			>> ${D}${sysconfdir}/directfbrc
	fi
	if [ ! -z "${DIRECTFB_PIXEL_FORMAT}" ]; then
		echo "pixelformat = ${DIRECTFB_PIXEL_FORMAT}" \
			>> ${D}${sysconfdir}/directfbrc
	fi
}

FILES_${PN} += " \
	${sysconfdir} \
	${@bb.utils.contains('PACKAGECONFIG', 'trace', '${libdir}/nm-n.*', '', d)} \
"
