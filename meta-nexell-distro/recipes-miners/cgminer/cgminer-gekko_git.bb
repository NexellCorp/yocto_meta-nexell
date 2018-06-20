include cgminer.inc

SUMMAR = "cgminer for the compac/2pac mining sticks"

SRC_URI = "git://gitlab.com/JinyongLee/cgminer-gekko.git;protocol=https;branch=master \
		   file://cgminer.conf.factory \
		   file://cgminer.sh \
		   file://cgminer.service \
"
FILES_${PN} += " ${sysconfdir} config"

#SRCREV = "8894aaf089b2c398337fd1ee000e306e6a001e4d"
SRCREV = "1fccd3d4b3e1a75659b6bb5376735870ace1d5c9"
PV = "4.9+gitr${SRCPV}"

PACKAGECONFIG_append 	= " gekko"
PACKAGECONFIG[gekko]  	= "--enable-gekko,,"

INITSCRIPT_NAME = "cgminer"
INITSCRIPT_PARAMS = "defaults 99"

# rename to gekko specific cgminer binary, so we can co-exist multiple versions
CFLAGS_prepend = "-I . -I ${S} -I ${S}/compat/jansson-2.6/src -I ${S}/compat/libusb-1.0/libusb"

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
