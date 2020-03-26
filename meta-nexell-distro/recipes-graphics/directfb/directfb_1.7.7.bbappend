FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI += " \
	file://configure-tslib.patch \
	file://fix-tslib-1.15-ts_config.patch \
	file://combine-drmkms_system-vt-header.patch \
	file://fix-build-error-use-PTHREAD_MUTEX_RECURSIVE.patch \
"

SRC_URI += " \
	file://directfbrc \
"

# "<true opt>,<false opt>,<depend package>"
DFB_PACKAGE_CONFIGURE ??= ",,"

PACKAGECONFIG[configure] = "${DFB_PACKAGE_CONFIGURE}"
PACKAGECONFIG[debug] = "--enable-debug-support --enable-debug,,"
PACKAGECONFIG[trace] = "--enable-trace,,"

# Default set drmkms for libdirectfb-nexell
PACKAGECONFIG_append = " drmkms"

do_install_append () {
	install -d ${D}${sysconfdir}
	install -m 644 ${WORKDIR}/directfbrc ${D}${sysconfdir}
}

FILES_${PN} += " \
	${sysconfdir} \
	${@bb.utils.contains('PACKAGECONFIG', 'trace', '${libdir}/nm-n.*', '', d)} \
"
