DESCRIPTION = "OPTEE BUILD for artik7"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://README.md;md5=871e3be979e189da85a03fe6ba47bbe2"

SRCREV = "9e859b45b131d07f5076d0d77d6b3a65f384b4e3"
SRC_URI = "git://git.nexell.co.kr/nexell/secure/optee/optee_build;protocol=git;branch=artik"

S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "artik710-raptor"

do_compile() {
    local uart_base=0xc00a3000
    
    oe_runmake -f ${WORKDIR}/git/Makefile clean
    oe_runmake -f ${WORKDIR}/git/Makefile PLAT_DRAM_SIZE=1024 PLAT_UART_BASE=${uart_base} build-bl1 -j8
    oe_runmake -f ${WORKDIR}/git/Makefile PLAT_DRAM_SIZE=1024 PLAT_UART_BASE=${uart_base} build-lloader -j8
    oe_runmake -f ${WORKDIR}/git/Makefile PLAT_DRAM_SIZE=1024 PLAT_UART_BASE=${uart_base} build-bl32 -j8
    oe_runmake -f ${WORKDIR}/git/Makefile PLAT_DRAM_SIZE=1024 PLAT_UART_BASE=${uart_base} build-fip -j8
    oe_runmake -f ${WORKDIR}/git/Makefile PLAT_DRAM_SIZE=1024 PLAT_UART_BASE=${uart_base} build-singleimage -j8

    echo -e "\033[44;33m[suker][optee_build]${WORKDIR}/git/\033[0m" >> ${TOPDIR}/mylog.txt
}