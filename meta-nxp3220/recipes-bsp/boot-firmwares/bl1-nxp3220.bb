DESCRIPTION = "Nexell BL1(bootloader) for NXP3220"
SECTION = "bootloaders"
LICENSE = "CLOSED"

inherit deploy
inherit externalsrc

EXTERNALSRC = "${BL1_SOURCE}"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""


do_install() {
	BIN=${BL1_BIN}

	install -d ${D}/${datadir}/${PN}
	install -m 0644 ${S}/${BIN} ${D}/${datadir}/${PN}
}

do_deploy () {
	BIN=${BL1_BIN}

	install -d ${DEPLOYDIR}
	install -m 0644 ${S}/${BIN} ${DEPLOYDIR}
}
addtask deploy before do_build after do_compile

# not execute tasks
do_configure[noexec] = "1"
do_compile[noexec] = "1"
do_package[noexec] = "1"
do_packagedata[noexec] = "1"
do_package_qa[noexec] = "1"
do_package_write_rpm[noexec] = "1"
do_populate_sysroot[noexec] = "1"

FILES_${PN}-dev = "${datadir}/${PN}"

PACKAGE_ARCH = "${MACHINE_ARCH}"
