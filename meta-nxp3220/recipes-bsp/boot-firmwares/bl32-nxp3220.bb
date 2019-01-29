DESCRIPTION = "Nexell BL32(Fake secure OS) for NXP3220"
SECTION = "bootloaders"
LICENSE = "CLOSED"

inherit deploy
inherit externalsrc

EXTERNALSRC = "${BL32_SOURCE}"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""

EXTRA_OEMAKE = "CROSS_COMPILE=${TARGET_PREFIX}"
PARALLEL_MAKE = "-j 1"

do_deploy () {
	NSIH=${BL32_NSIH}
	BINGEN=${TOOL_BINGEN}
	AESCBC=${TOOL_BINENC}
	BOOTKEY=${BL32_BOOTKEY}
	USERKEY=${BL32_USERKEY}
	AESKEY=${BL32_AESKEY}
	AESVECTOR=${BL32_VECTOR}
	BIN=${BL32_BIN}

	install -m 0644 ${S}/out/${BIN} ${DEPLOYDIR};
	${AESCBC} -n ${DEPLOYDIR}/${BIN} \
		-k $(cat ${AESKEY}) -v $(cat ${AESVECTOR}) -m enc -b 128;
	${BINGEN} -n ${NSIH} -i ${DEPLOYDIR}/${BIN}.enc \
		-b ${BOOTKEY} -u ${USERKEY} -k bl32 -l ${BL32_LOADADDR} -s ${BL32_LOADADDR} -t;
	${BINGEN} -n ${NSIH} -i ${DEPLOYDIR}/${BIN} \
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
