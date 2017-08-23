DESCRIPTION = "Nexell customize files for rootfs"
SECTION = "console/utils"

LICENSE = "CLOSED"

inherit linux-nexell-base
ARCH_TYPE_NUM = "${@get_kernel_arch_num(d,"${TARGET_PREFIX}")}"

SRC_URI = " \
    file://32/usr/sbin \
    "

S = "${WORKDIR}"

do_install() {
    #/usr/sbin
    install -d ${D}${sbindir}

    if [ "${ARCH_TYPE_NUM}" -eq "32" ]; then
        install -m 0755 ${WORKDIR}/${ARCH_TYPE_NUM}/usr/sbin/nx_reboot    ${D}${sbindir}/
    else
        echo "Requires 64bit binaries."
    fi
}

FILES_${PN} = "${sbindir}"
