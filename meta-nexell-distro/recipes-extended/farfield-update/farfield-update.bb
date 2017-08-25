DESCRIPTION = "Nexell customize files for farfield"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://usr/bin \
    "

S = "${WORKDIR}"

do_install() {
    #/usr/bin
    install -d ${D}${bindir}

    install -m 0755 ${WORKDIR}/usr/bin/* ${D}${bindir}/
}

FILES_${PN} = "${bindir}"

INSANE_SKIP_${PN} = "already-stripped"