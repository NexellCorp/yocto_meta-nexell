### For U-BOOT tools
#require u-boot-nxp3220.inc

SUMMARY = "U-Boot bootloader image creation tool"
#PROVIDES = "u-boot-mkimage u-boot-mkimage-native"

DEPENDS += "u-boot"

LICENSE = "GPLv2+"
PACKAGE_ARCH = "${MACHINE_ARCH}" 

EXTRA_OEMAKE_class-native = 'CC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}" HOSTCC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}" STRIP=true V=1'
EXTRA_OEMAKE_class-nativesdk = 'CROSS_COMPILE="${HOST_PREFIX}" CC="${CC} ${CFLAGS} ${LDFLAGS}" HOSTCC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}" STRIP=true V=1'

inherit externalsrc
EXTERNALSRC = "${BSP_ROOT_DIR}/u-boot-2018.5"
EXTERNALSRC_BUILD = "${EXTERNALSRC}"
EXTERNALSRC_SYMLINKS = ""

do_install () {
	install -d ${D}${bindir}
	install -m 0755 tools/mkimage ${D}${bindir}/uboot-mkimage
	ln -sf uboot-mkimage ${D}${bindir}/mkimage
}

BBCLASSEXTEND = "native nativesdk"

# not execute tasks
do_unpack[noexec] = "1"
do_patch[noexec] = "1" 
do_checkuri[noexec] = "1"
do_configure[noexec] = "1" 
do_compile[noexec] = "1" 
