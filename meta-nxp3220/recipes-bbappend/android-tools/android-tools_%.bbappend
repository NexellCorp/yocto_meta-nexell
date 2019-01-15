FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI += " \
	file://adbd-property_get-ro-secure-disable.patch \
"

SRC_URI += " \
	file://android-tools-adbd.service \
	file://start_adbd.sh \
	file://stop_adbd.sh \
"

do_install_append() {
    # skip native build system
    if [ -z ${TARGET_PREFIX} ]; then
	return
    fi

    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/android-tools-adbd.service ${D}${systemd_unitdir}/system/

    install -m 0755 ${WORKDIR}/start_adbd.sh ${D}${bindir}/start_adbd.sh
    install -m 0755 ${WORKDIR}/stop_adbd.sh ${D}${bindir}/
}

FILES_${PN} = "${bindir} ${base_sbindir}"
