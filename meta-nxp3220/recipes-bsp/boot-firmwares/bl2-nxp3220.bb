### For BL2
DESCRIPTION = "Nexell BL2(bootloader) for NXP3220"
SECTION = "bootloaders"
LICENSE = "CLOSED"

inherit deploy
inherit externalsrc
inherit nexell-bingen

EXTERNALSRC = "${BL2_SOURCE}"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""

EXTRA_OEMAKE = "CROSS_COMPILE=${TARGET_PREFIX} CHIPNAME=${BL2_CHIP} BOARD=${BL2_BOARD} PMIC=${BL2_PMIC}"
PARALLEL_MAKE = "-j 1"

do_deploy () {
	install -d ${DEPLOYDIR}
	install -m 0644 ${S}/out/${BL2_BIN} ${DEPLOYDIR}
	install -m 0644 ${SECURE_BOOTKEY} ${DEPLOYDIR}
	install -m 0644 ${SECURE_USERKEY} ${DEPLOYDIR}

	bootkey=${DEPLOYDIR}/$(basename ${SECURE_BOOTKEY})
	userkey=${DEPLOYDIR}/$(basename ${SECURE_USERKEY})

	# Binary + NSIH : $BIN.raw
	do_bingen_raw bl2 ${DEPLOYDIR}/${BL2_BIN} \
		${BL2_NSIH} ${bootkey} ${userkey} ${BL2_LOADADDR};

	cp ${DEPLOYDIR}/${BL2_BIN}.raw ${DEPLOYDIR}/bl2.bin.raw;

	if ${@bb.utils.contains('BINARY_FEATURES','nand.ecc','true','false',d)}; then
		if [ -z ${FLASH_PAGE_SIZE} ]; then
			echo "ERROR: NOT Defined 'FLASH_PAGE_SIZE'"
			exit 1
		fi

		# (Binary + NSIH) + ECC : $BIN.raw.ecc
		do_bingen_ecc ${DEPLOYDIR}/${BL2_BIN}.raw ${FLASH_PAGE_SIZE}

		cp ${DEPLOYDIR}/${BL2_BIN}.raw.ecc ${DEPLOYDIR}/bl2.bin.raw.ecc;
	fi
}

addtask deploy before do_build after do_compile

# not execute tasks
deltask do_packagedata
deltask do_package_qa
deltask do_package_write_rpm
deltask do_populate_sysroot

# for secure image
do_deploy[nostamp] = "1"

FILES_${PN}-dev = "${datadir}/${PN}"

PACKAGE_ARCH = "${MACHINE_ARCH}"
