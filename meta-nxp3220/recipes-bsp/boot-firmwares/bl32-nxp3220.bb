DESCRIPTION = "Nexell BL32(Fake secure OS) for NXP3220"
SECTION = "bootloaders"
LICENSE = "CLOSED"

inherit deploy
inherit externalsrc

EXTERNALSRC = "${BSP_ROOT_DIR}/firmwares/bl32-nxp3220"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""

EXTRA_OEMAKE = "CROSS_COMPILE=${TARGET_PREFIX}"
PARALLEL_MAKE = "-j 1"

TOOL_DIR = "${BSP_ROOT_DIR}/tools"
FILE_DIR = "${TOOL_DIR}/files"
BL32_DIR = "${EXTERNALSRC}"

BINGEN= "${TOOL_DIR}/bin/bingen"
AESCBC= "${TOOL_DIR}/bin/aescbc_enc"

NSIH = "${BL32_DIR}/reference-nsih/${BL32_NSIH}"
BOOTKEY = "${FILE_DIR}/${BL32_BOOTKEY}"
USERKEY = "${FILE_DIR}/${BL32_USERKEY}"
AESKEY = "${FILE_DIR}/${BL32_AESKEY}"
AESVECTOR = "${FILE_DIR}/${BL32_VECTOR}"

BL32_BIN = "bl32.bin"

do_deploy () {
	install -m 0644 ${S}/out/${BL32_BIN} ${DEPLOYDIR};
	${AESCBC} -n ${DEPLOYDIR}/${BL32_BIN} \
		-k $(cat ${AESKEY}) -v $(cat ${AESVECTOR}) -m enc -b 128;
	${BINGEN} -n ${NSIH} -i ${DEPLOYDIR}/${BL32_BIN}.enc \
		-b ${BOOTKEY} -u ${USERKEY} -k bl32 -l ${BL32_LOADADDR} -s ${BL32_LOADADDR} -t;
	${BINGEN} -n ${NSIH} -i ${DEPLOYDIR}/${BL32_BIN} \
		-b ${BOOTKEY} -u ${USERKEY} -k bl32 -l ${BL32_LOADADDR} -s ${BL32_LOADADDR} -t
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
