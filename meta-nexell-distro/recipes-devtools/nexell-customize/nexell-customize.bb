DESCRIPTION = "Nexell customize files for ramdisk"
SECTION = "console/utils"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://etc \
    file://sbin \
    file://usr/bin \
    "

S = "${WORKDIR}"

do_install() {
    #/etc
    install -d ${D}${sysconfdir}
    #/usr/bin
    install -d ${D}${bindir}
    #sbin
    install -d ${D}${base_sbindir}
    #/usr/sbin
    #install -d ${D}${sbindir}

    if [ "${ARCH_TYPE_NUM}" -eq "32" ]; then
        install -m 0644 ${WORKDIR}/etc/*     ${D}${sysconfdir}/
        install -m 0755 ${WORKDIR}/usr/bin/* ${D}${bindir}/
        install -m 0755 ${WORKDIR}/sbin/*    ${D}${base_sbindir}/
    fi
}

FILES_${PN} = "${bindir} ${sysconfdir} ${base_sbindir}"