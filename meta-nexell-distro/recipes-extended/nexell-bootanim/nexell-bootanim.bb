CRIPTION = "Nexell customize files for bootanimation"
SECTION = "console/utils"

LICENSE = "CLOSED"

inherit linux-nexell-base
ARCH_TYPE_NUM = "${@get_kernel_arch_num(d,"${TARGET_PREFIX}")}"

SRC_URI = " \
    file://32/usr/bin \
    file://32/etc/qboot \
    file://32/system/media \
    "

S = "${WORKDIR}"

do_install() {
    #/usr/sbin
    install -d ${D}${bindir}
    install -d ${D}${sysconfdir}/qboot
    install -d ${D}/system/media

    if [ "${ARCH_TYPE_NUM}" -eq "32" ]; then
        install -m 0755 ${WORKDIR}/${ARCH_TYPE_NUM}/usr/bin/bootanimation          ${D}${bindir}/
        install -m 0755 ${WORKDIR}/${ARCH_TYPE_NUM}/etc/qboot/animation.conf       ${D}${sysconfdir}/qboot
        install -m 0755 ${WORKDIR}/${ARCH_TYPE_NUM}/system/media/bootanimation.zip ${D}/system/media
    else
        echo "Requires 64bit binaries."
    fi
}

FILES_${PN} = "${bindir} ${sysconfdir}/qboot system/media"
