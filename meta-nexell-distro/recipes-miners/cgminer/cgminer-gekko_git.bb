include cgminer.inc

SUMMARY = "cgminer for the compac/2pac mining sticks"

SRC_URI = "git://git.nexell.co.kr/nexell/bitcoin/cgminer;protocol=git;branch=nexell \
"
SRC_URI_append = " \
		   file://cgminer.conf.factory \
		   file://cgminer.sh \
		   file://cgminer.service \
"

FILES_${PN} += " ${sysconfdir} config"


#SRCREV = "8894aaf089b2c398337fd1ee000e306e6a001e4d"
#SRCREV = "4740a10e3b367c63928ac7a5bde25835b6a8ae7b"
SRCREV = "${AUTOREV}"
#PV = "4.9+gitr${SRCPV}"

#PACKAGECONFIG_append 	= " gekko"
#PACKAGECONFIG[gekko]  	= "--enable-gekko,,"
PACKAGECONFIG_append 	= " tsb1101"
PACKAGECONFIG[tsb1101] 	= "--enable-tsb1101,,"

INITSCRIPT_NAME = "cgminer"
INITSCRIPT_PARAMS = "defaults 99"

# rename to gekko specific cgminer binary, so we can co-exist multiple versions
#CFLAGS_prepend = "-I . -I ${S} -I ${S}/compat/jansson-2.6/src -I ${S}/compat/libusb-1.0/libusb"
CFLAGS_prepend = "-I . -I ${S} -I ${S}/compat/jansson-2.9/src `pkg-config libusb-1.0 --cflags`"

do_compile_append() {
	    make api-example
}

do_install_append() {
	install -m 0755 ${B}/api-example ${D}/usr/bin/cgminer-api
	install -d ${D}/config ${D}${sysconfdir}/init.d
	install -m 0755 ${WORKDIR}/cgminer.sh ${D}${sysconfdir}/init.d
	install -m 0400 ${WORKDIR}/cgminer.conf.factory ${D}/config/cgminer.conf
	install -d ${D}${sysconfdir}
	install -m 0400 ${WORKDIR}/cgminer.conf.factory ${D}${sysconfdir}
}
