### For BL1
DESCRIPTION = "Nexell BL1(bootloader) for NXP3220"
SECTION = "bootloaders"
LICENSE = "CLOSED"

inherit deploy
inherit externalsrc
inherit nexell-bingen

EXTERNALSRC = "${BL1_SOURCE}"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""

SRC_URI = " file://secure-bl1-ivector.txt "

SECURE_BL1_IVECTOR = "${WORKDIR}/secure-bl1-ivector.txt"

do_install() {
	install -d ${D}/${datadir}/${PN}
	install -m 0644 ${S}/${BL1_BIN} ${D}/${datadir}/${PN}
}

do_deploy () {
	install -d ${DEPLOYDIR}
	install -m 0644 ${S}/${BL1_BIN} ${DEPLOYDIR}

	# Encrypt binary : $BIN.enc
	do_bingen_enc ${DEPLOYDIR}/${BL1_BIN} ${SECURE_BL1_ENCKEY} ${SECURE_BL1_IVECTOR} "128";

	# (Encrypted binary) + NSIH : $BIN.bin.enc.raw
	do_bingen_raw bl1 ${DEPLOYDIR}/${BL1_BIN}.enc \
		${BL1_NSIH} ${SECURE_BOOTKEY} ${SECURE_USERKEY} ${BL1_LOADADDR};

	# Binary + NSIH : $BIN.raw
	do_bingen_raw bl1 ${DEPLOYDIR}/${BL1_BIN} \
		${BL1_NSIH} ${SECURE_BOOTKEY} ${SECURE_USERKEY} ${BL1_LOADADDR};

	if ${@bb.utils.contains('BINARY_FEATURES','nand.ecc','true','false',d)}; then
		if [ -z ${FLASH_PAGE_SIZE} ]; then
			echo "ERROR: NOT Defined 'FLASH_PAGE_SIZE'"
			exit 1
		fi

		# ((Encrypted Binary) + NSIH) + ECC : $BIN.enc.raw.ecc
		do_bingen_ecc ${DEPLOYDIR}/${BL1_BIN}.enc.raw ${FLASH_PAGE_SIZE}

		# (Binary + NSIH) + ECC : $BIN.raw.ecc
		do_bingen_ecc ${DEPLOYDIR}/${BL1_BIN}.raw ${FLASH_PAGE_SIZE}
	fi
}

addtask deploy before do_build after do_compile

# not execute tasks
#deltask do_configure
#deltask do_compile
deltask do_packagedata
deltask do_package_qa
deltask do_package_write_rpm
deltask do_populate_sysroot

FILES_${PN}-dev = "${datadir}/${PN}"

PACKAGE_ARCH = "${MACHINE_ARCH}"
