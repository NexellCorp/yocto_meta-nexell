DESCRIPTION = "Nexell customize files for ramdisk"
SECTION = "console/utils"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit linux-nexell-base
ARCH_TYPE_NUM = "${@get_kernel_arch_num(d,"${TARGET_PREFIX}")}"

SRC_URI = " \
    file://32/etc \
    file://32/sbin \
    file://32/usr/bin \
    file://32/lib/systemd \
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

    if [ "${ARCH_TYPE_NUM}" -eq "32" ]; then
        install -m 0755 ${WORKDIR}/${ARCH_TYPE_NUM}/etc/*     ${D}${sysconfdir}/
        install -m 0755 ${WORKDIR}/${ARCH_TYPE_NUM}/usr/bin/* ${D}${bindir}/
        install -m 0755 ${WORKDIR}/${ARCH_TYPE_NUM}/sbin/*    ${D}${base_sbindir}/
        install -m 0644 ${WORKDIR}/${ARCH_TYPE_NUM}/lib/systemd/system/nxupdate.service ${D}${systemd_unitdir}/system/

        cd ${D}${systemd_unitdir}/system/multi-user.target.wants
        ln -sf ../nxupdate.service nxupdate.service
    else
        echo "Requires 64bit binaries."
    fi
}

INSANE_SKIP_${PN} = "file-rdeps"
FILES_${PN} = "${bindir} ${sysconfdir} ${base_sbindir} ${systemd_unitdir}"
