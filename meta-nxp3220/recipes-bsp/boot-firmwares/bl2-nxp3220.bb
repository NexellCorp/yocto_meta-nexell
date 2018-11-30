DESCRIPTION = "Nexell BL2(bootloader) for NXP3220"
SECTION = "bootloaders"
LICENSE = "CLOSED"

inherit deploy
inherit externalsrc

EXTERNALSRC = "${BSP_ROOT_DIR}/firmwares/bl2-nxp3220"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""

EXTRA_OEMAKE = "CROSS_COMPILE=${TARGET_PREFIX} CHIPNAME=${BL2_CHIP} BOARD=${BL2_BOARD} PMIC=${BL2_PMIC}"
PARALLEL_MAKE = "-j 1"

TOOL_DIR = "${BSP_ROOT_DIR}/tools"
FILE_DIR = "${TOOL_DIR}/files"
BL2_DIR = "${EXTERNALSRC}"

BINGEN= "${TOOL_DIR}/bin/bingen"
NSIH = "${BL2_DIR}/reference-nsih/${BL2_NSIH}"
BOOTKEY = "${FILE_DIR}/${BL2_BOOTKEY}"
USERKEY = "${FILE_DIR}/${BL2_USERKEY}"

do_deploy () {
	install -m 0644 ${S}/out/${BL2_BIN} ${DEPLOYDIR};
	${BINGEN} -n ${NSIH} -i ${DEPLOYDIR}/${BL2_BIN} \
		-b ${BOOTKEY} -u ${USERKEY} -k bl2 -l 0xFFFF9000 -s 0xFFFF9000 -t;
	cp ${DEPLOYDIR}/${BL2_BIN}.raw ${DEPLOYDIR}/bl2.bin.raw;
}
addtask deploy before do_build after do_compile

# not execute tasks
do_package[noexec] = "1"
do_packagedata[noexec] = "1"
do_package_qa[noexec] = "1"
do_package_write_rpm[noexec] = "1"
do_populate_sysroot[noexec] = "1"

FILES_${PN}-dev = "${datadir}/${PN}"

PACKAGE_ARCH = "${MACHINE_ARCH}"
