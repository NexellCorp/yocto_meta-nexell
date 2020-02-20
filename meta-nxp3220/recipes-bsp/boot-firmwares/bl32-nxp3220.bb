### For BL32
DESCRIPTION = "Nexell BL32(Fake secure OS) for NXP3220"
SECTION = "bootloaders"
LICENSE = "CLOSED"

inherit deploy
inherit externalsrc
inherit nexell-bingen

EXTERNALSRC = "${BL32_SOURCE}"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""

SRC_URI = " file://secure-bl1-ivector.txt "

SECURE_BL32_IVECTOR = "${WORKDIR}/secure-bl1-ivector.txt"

EXTRA_OEMAKE = "CROSS_COMPILE=${TARGET_PREFIX}"
PARALLEL_MAKE = "-j 1"

do_deploy () {
        install -d ${DEPLOYDIR}
        install -m 0644 ${S}/out/${BL32_BIN} ${DEPLOYDIR}
	install -m 0644 ${SECURE_BL32_ENCKEY} ${DEPLOYDIR}
	install -m 0644 ${SECURE_BOOTKEY} ${DEPLOYDIR}
	install -m 0644 ${SECURE_USERKEY} ${DEPLOYDIR}

	aeskey=${DEPLOYDIR}/$(basename ${SECURE_BL32_ENCKEY})
	bootkey=${DEPLOYDIR}/$(basename ${SECURE_BOOTKEY})
	userkey=${DEPLOYDIR}/$(basename ${SECURE_USERKEY})

	# Encrypt binary : $BIN.enc
        do_bingen_enc ${DEPLOYDIR}/${BL32_BIN} ${aeskey} ${SECURE_BL32_IVECTOR} "128";

	# (Encrypted binary) + NSIH : $BIN.enc.raw
        do_bingen_raw bl32 ${DEPLOYDIR}/${BL32_BIN}.enc \
		${BL32_NSIH} ${bootkey} ${userkey} ${BL32_LOADADDR} "-e";

	# Binary + NSIH : $BIN.raw
        do_bingen_raw bl32 ${DEPLOYDIR}/${BL32_BIN} \
		${BL32_NSIH} ${bootkey} ${userkey} ${BL32_LOADADDR};

	if ${@bb.utils.contains('BINARY_FEATURES','nand.ecc','true','false',d)}; then
		if [ -z ${FLASH_PAGE_SIZE} ]; then
			echo "ERROR: NOT Defined 'FLASH_PAGE_SIZE'"
			exit 1
		fi

		# ((Encrypted Binary) + NSIH) + ECC : $BIN.enc.raw.ecc
		do_bingen_ecc ${DEPLOYDIR}/${BL32_BIN}.enc.raw ${FLASH_PAGE_SIZE}

		# (Binary + NSIH) + ECC : $BIN.raw.ecc
		do_bingen_ecc ${DEPLOYDIR}/${BL32_BIN}.raw ${FLASH_PAGE_SIZE}
	fi
}

addtask deploy before do_build after do_compile

# not execute tasks
deltask do_packagedata
deltask do_package_qa
deltask do_package_write_rpm
deltask do_populate_sysroot

FILES_${PN}-dev = "${datadir}/${PN}"

PACKAGE_ARCH = "${MACHINE_ARCH}"
