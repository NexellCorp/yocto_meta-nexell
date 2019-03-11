inherit nexell-u-boot

HOMEPAGE = "http://www.denx.de/wiki/U-Boot/WebHome"
SECTION = "bootloaders"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=a2c678cfd4a4d97135585cad908541c6"

DEPENDS += "dtc-native"
DEPENDS += "bl1-nxp3220 bl2-nxp3220 bl32-nxp3220"
PROVIDES += "u-boot"

# externalsrc
# EXTERNALSRC_BUILD = "${WORKDIR}/build"
# EXTERNALSRC_BUILD = "${EXTERNALSRC}"
inherit externalsrc
EXTERNALSRC = "${BSP_ROOT_DIR}/u-boot-2018.5"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""

# This allows the kernel to be incrementally build within the shell
# and not dirty the source directory
KBUILD_OUTPUT = "${B}"
OE_TERMINAL_EXPORTS += "KBUILD_OUTPUT"

do_configure_prepend () {
    if [ "${EXTERNALSRC}" != "${EXTERNALSRC_BUILD}" ]; then
	oe_runmake -C ${S} distclean
    else
        file=${B}/.uboot_defconfig
	if [ -e ${file} ]; then
	    conf=$(cat ${file})
	    if [ "${conf}" != "${UBOOT_MACHINE}" ]; then
		rm ${file}; echo ${UBOOT_MACHINE} >> ${file};
	        oe_runmake -C ${S} distclean
	    fi
	else
	    echo ${UBOOT_MACHINE} >> ${file};
	    oe_runmake -C ${S} distclean
	fi
    fi

    if [ ! -f ${B}/.config ]; then
	oe_runmake -C ${S} O=${B} ${UBOOT_MACHINE}
    fi
}

do_compile_append() {
	find ./env -name "common.o"

	if [ $? -eq 0 ]; then
		cp `find ./env -name "common.o"` copy_env_common.o
		SECTION_NAME=".rodata.default_environment"
		set +e
		SECTION_ENV=`${OBJDUMP} -h copy_env_common.o | grep $SECTION_NAME`
		if [ "$SECTION_ENV" = "" ]; then
			SECTION_NAME=".rodata"
		fi
		${OBJCOPY} -O binary --only-section=${SECTION_NAME} `find . -name "copy_env_common.o"`
		${READELF} -s `find ./env -name "common.o"` | \
			grep default_environment | \
			awk '{print "dd if=./copy_env_common.o of=./default_env.bin bs=1 skip=$[0x" $2 "] count=" $3 }' | \
			bash
		sed -e '$!s/$/\\/g;s/\x0/\n/g' ./default_env.bin | \
			tee params_env.txt > /dev/null

		tools/mkenvimage -s 16384 -o params_env.bin params_env.txt
	fi
}

do_deploy_append () {
	NSIH=${UBOOT_NSIH}
	BINGEN=${TOOL_BINGEN}
	BIN=${UBOOT_BIN}
	BOOTKEY=${UBOOT_BOOTKEY}
	USERKEY=${UBOOT_USERKEY}

	${BINGEN} -n ${NSIH} -i ${B}/${BIN} \
		-b ${BOOTKEY} -u ${USERKEY} \
		-k bl33 -l ${UBOOT_LOADADDR} -s ${UBOOT_LOADADDR} -t;
	install -m 0644 ${B}/${BIN}.* ${DEPLOYDIR}

	if [ -f ${B}/params_env.txt ]; then
		install -m 0644 ${B}/params_env.txt ${DEPLOYDIR}
	fi
	if [ -f ${B}/params_env.bin ]; then
		install -m 0644 ${B}/params_env.bin ${DEPLOYDIR}
	fi
}

# not execute tasks
do_package[noexec] = "1"
do_packagedata[noexec] = "1"
do_package_qa[noexec] = "1"
do_package_write_rpm[noexec] = "1"
do_populate_sysroot[noexec] = "1"
