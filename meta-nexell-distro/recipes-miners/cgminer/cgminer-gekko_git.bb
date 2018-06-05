include cgminer.inc

SUMMAR = "cgminer for the compac/2pac mining sticks"

SRC_URI = "git://gitlab.com/JinyongLee/cgminer-gekko.git;protocol=https;branch=master"
SRCREV = "1fccd3d4b3e1a75659b6bb5376735870ace1d5c9"
PV = "4.9+gitr${SRCPV}"

PACKAGECONFIG_append 	= " gekko"
PACKAGECONFIG[gekko]  	= "--enable-gekko,,"

# rename to gekko specific cgminer binary, so we can co-exist multiple versions
do_install_append() {
	mv ${D}/usr/bin/cgminer ${D}/usr/bin/${PN}
}
