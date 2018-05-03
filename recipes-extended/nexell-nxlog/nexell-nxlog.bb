DESCRIPTION = "Nexell Logging System"
SECTION = "console/utils"

LICENSE = "CLOSED"

inherit linux-nexell-base
ARCH_TYPE_NUM = "${@get_kernel_arch_num(d,"${TARGET_PREFIX}")}"

SRC_URI = " \
    file://32/usr/bin \
    file://32/usr/sbin \
    file://32/lib/systemd \
    "

S = "${WORKDIR}"

do_install() {
    #/usr/bin
    install -d ${D}${bindir}

    #/usr/sbin
    install -d ${D}${sbindir}

    #/lib/systemd/system/multi-user.target.wants
    install -d ${D}${systemd_unitdir}/system/ ${D}${systemd_unitdir}/system/multi-user.target.wants/

    if [ "${ARCH_TYPE_NUM}" -eq "32" ]; then
        install -m 0755 ${WORKDIR}/${ARCH_TYPE_NUM}/usr/bin/nxlogcat     ${D}${bindir}/
        cp -aR ${WORKDIR}/${ARCH_TYPE_NUM}/usr/bin/logcat ${D}${bindir}/
        install -m 0755 ${WORKDIR}/${ARCH_TYPE_NUM}/usr/sbin/nxlogrotate ${D}${sbindir}/
        install -m 0644 ${WORKDIR}/${ARCH_TYPE_NUM}/lib/systemd/system/nxlogrotate.service ${D}${systemd_unitdir}/system/
        cp -aR ${WORKDIR}/${ARCH_TYPE_NUM}/lib/systemd/system/multi-user.target.wants/nxlogrotate.service ${D}${systemd_unitdir}/system/multi-user.target.wants/
    else
        echo "Requires 64bit binaries."
    fi
}

FILES_${PN} = "${bindir} ${sbindir} ${systemd_unitdir}"
