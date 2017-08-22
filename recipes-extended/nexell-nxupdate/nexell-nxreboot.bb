DESCRIPTION = "Nexell customize files for rootfs"
SECTION = "console/utils"

LICENSE = "CLOSED"

SRC_URI = " \
    file://usr/sbin \
    "

S = "${WORKDIR}"

do_install() {
    #/usr/sbin
    install -d ${D}${sbindir}

    install -m 0755 ${WORKDIR}/usr/sbin/nx_reboot    ${D}${sbindir}/
}

FILES_${PN} = "${sbindir}"
