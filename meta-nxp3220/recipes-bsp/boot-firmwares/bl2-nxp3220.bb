DESCRIPTION = "Nexell BL2(bootloader) for NXP3220"
SECTION = "bootloaders"
LICENSE = "CLOSED"

inherit deploy
inherit externalsrc

EXTERNALSRC = "${BL2_SOURCE}"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""

EXTRA_OEMAKE = "CROSS_COMPILE=${TARGET_PREFIX} CHIPNAME=${BL2_CHIP} BOARD=${BL2_BOARD} PMIC=${BL2_PMIC}"
PARALLEL_MAKE = "-j 1"

do_deploy () {
	BINGEN=${TOOL_BINGEN}
	NSIH=${BL2_NSIH}
	BOOTKEY=${BL2_BOOTKEY}
	USERKEY=${BL2_USERKEY}
	BIN=${BL2_BIN}

	install -m 0644 ${S}/out/${BIN} ${DEPLOYDIR};
	${BINGEN} -n ${NSIH} -i ${DEPLOYDIR}/${BIN} \
		-b ${BOOTKEY} -u ${USERKEY} -k bl2 -l 0xFFFF9000 -s 0xFFFF9000 -t;
	cp ${DEPLOYDIR}/${BIN}.raw ${DEPLOYDIR}/bl2.bin.raw;
}
addtask deploy before do_build after do_compile

# not execute tasks
do_packagedata[noexec] = "1"
do_package_qa[noexec] = "1"
do_package_write_rpm[noexec] = "1"
do_populate_sysroot[noexec] = "1"

FILES_${PN}-dev = "${datadir}/${PN}"

PACKAGE_ARCH = "${MACHINE_ARCH}"
