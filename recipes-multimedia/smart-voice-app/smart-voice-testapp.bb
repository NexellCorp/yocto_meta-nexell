SUMMARY = "Nexell smart voice test application"
LICENSE = "CLOSED"

SRC_URI = "file://Makefile \
           file://lib \
           file://src \
"

S = "${WORKDIR}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} CC="$CC" clean
    oe_runmake CROSS_COMPILE=${TARGET_PREFIX} LDFLAGS+=" -mfloat-abi=hard --sysroot=${STAGING_DIR_HOST} " CC="${CC}" \
               INCLUDE="-I${STAGING_INCDIR} \
                        -I${S}/src \
                        -I${S}/lib \
                        -I${STAGING_INCDIR}/c++/5.3.0 \
                        -I${STAGING_INCDIR}/c++/5.3.0/${HOST_SYS}/bits \
                        -I${STAGING_INCDIR}/c++/5.3.0/${HOST_SYS}"              
}

do_install() {
    install -d ${D}${bindir}

    install -m 0755 ${S}/src/smart_voice ${D}${bindir}
}

FILES_${PN} = "${bindir} "