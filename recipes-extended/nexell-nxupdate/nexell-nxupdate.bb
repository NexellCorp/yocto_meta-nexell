DESCRIPTION = "Nexell customize files for ramdisk"
SECTION = "console/utils"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://etc \
    file://sbin \
    file://usr/bin \
	file://lib/systemd \
    "

S = "${WORKDIR}"

do_install() {
    #/etc
    install -d ${D}${sysconfdir}

    #/usr/bin
    install -d ${D}${bindir}

    #/sbin
    install -d ${D}${base_sbindir}

	#/lib/systemd/system/multi-user.target.wants
	install -d ${D}${systemd_unitdir}/system/ ${D}${systemd_unitdir}/system/multi-user.target.wants/

    install -m 0755 ${WORKDIR}/etc/*     ${D}${sysconfdir}/
    install -m 0755 ${WORKDIR}/usr/bin/* ${D}${bindir}/
    install -m 0755 ${WORKDIR}/sbin/*    ${D}${base_sbindir}/
	install -m 0644 ${WORKDIR}/lib/systemd/system/nxupdate.service ${D}${systemd_unitdir}/system/

	ln -sf ${D}${systemd_unitdir}/system/nxupdate.service ${D}${systemd_unitdir}/system/multi-user.target.wants/nxupdate.service
}

FILES_${PN} = "${bindir} ${sysconfdir} ${base_sbindir} ${systemd_unitdir}"
