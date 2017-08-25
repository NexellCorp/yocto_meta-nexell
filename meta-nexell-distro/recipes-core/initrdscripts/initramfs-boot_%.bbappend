FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI = "file://init-boot-nexell.sh"

do_install() {
        install -m 0755 ${WORKDIR}/init-boot-nexell.sh ${D}/init
}
