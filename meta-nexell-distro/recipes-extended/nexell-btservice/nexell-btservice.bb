DESCRIPTION = "Nexell BT Service"
SECTION = "console/utils"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit linux-nexell-base
ARCH_TYPE_NUM = "${@get_kernel_arch_num(d,"${TARGET_PREFIX}")}"

SRC_URI = " \
    file://32/lib/systemd \
    "

S = "${WORKDIR}"

do_install() {
    #/lib/systemd/system/multi-user.target.wants
    install -d ${D}${systemd_unitdir}/system/ ${D}${systemd_unitdir}/system/multi-user.target.wants/

    if [ "${ARCH_TYPE_NUM}" -eq "32" ]; then
        install -m 0644 ${WORKDIR}/${ARCH_TYPE_NUM}/lib/systemd/system/bsa.service ${D}${systemd_unitdir}/system/
        cp -aR ${WORKDIR}/${ARCH_TYPE_NUM}/lib/systemd/system/bsa.service ${D}${systemd_unitdir}/system/multi-user.target.wants/
    else
        echo "Requires 64bit binaries."
    fi
}

FILES_${PN} = "${systemd_unitdir}"
