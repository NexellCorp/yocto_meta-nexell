SUMMARY = "This is a multi-threaded multi-pool FPGA and ASIC miner for bitcoin."
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://COPYING;md5=d32239bcb673463ab874e80d47fae504"

DEPENDS = "ncurses curl libusb jansson"

S = "${WORKDIR}/git"
PV = "NEXELL"
PR = "0.1"

inherit autotools pkgconfig systemd

PACKAGECONFIG ?= "jansson"

PACKAGECONFIG[ants1]  		= "--enable-ants1,,"
PACKAGECONFIG[ants2]  		= "--enable-ants2,,"
PACKAGECONFIG[avalon]  		= "--enable-avalon,,"
PACKAGECONFIG[avalon2]  	= "--enable-avalon2,,"
PACKAGECONFIG[avalon4]  	= "--enable-avalon4,,"
PACKAGECONFIG[avalon7]  	= "--enable-avalon7,,"
PACKAGECONFIG[bab]  		= "--enable-bab,,"
PACKAGECONFIG[bflsc]  		= "--enable-bflsc,,"
PACKAGECONFIG[bitforce]  	= "--enable-bitforce,,"
PACKAGECONFIG[bitfury]  	= "--enable-bitfury,,"
PACKAGECONFIG[bitmine_A1]  	= "--enable-bitmine_A1,,"
PACKAGECONFIG[blockerupter] = "--enable-blockerupter,,"
PACKAGECONFIG[cointerra] 	= "--enable-cointerra,,"
PACKAGECONFIG[drillbit]  	= "--enable-drillbit,,"
PACKAGECONFIG[dragonmint_t1]   = "--enable-dragonmint_t1,,"
PACKAGECONFIG[hashfast]  	= "--enable-hashfast,,"
PACKAGECONFIG[icarus]  		= "--enable-icarus,,"
PACKAGECONFIG[klondike]  	= "--enable-klondike,,"
PACKAGECONFIG[minion]  		= "--enable-minion,,"
PACKAGECONFIG[modminer]  	= "--enable-modminer,,"
PACKAGECONFIG[sp10]  		= "--enable-sp10,,"
PACKAGECONFIG[sp30]  		= "--enable-sp30,,"
PACKAGECONFIG[libcurl]  	= "--disable-libcurl,,"
PACKAGECONFIG[libsystemd]  	= "--enable-libsystemd,,"
PACKAGECONFIG[wo-curses] 	= "--without-curses,,"
PACKAGECONFIG[jansson]  	= "--with-system-jansson,,"

SYSTEMD_SERVICE_${PN} = "cgminer.service checkminerfs.service"

do_install_append() {
    install -d ${D}${bindir}
	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/cgminer.service ${D}${systemd_unitdir}/system
	sed -i -e 's,@SBINDIR@,${sbindir},g' \
		-e 's,@BINDIR@,${bindir},g' \
		-e 's,@SYSCONFDIR@,${sysconfdir},g' \
		-e 's,@BASE_BINDIR@,${base_bindir},g' \
		${D}${systemd_unitdir}/system/cgminer.service

	install -d ${D}${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/checkminerfs.service ${D}${systemd_unitdir}/system
	sed -i -e 's,@SBINDIR@,${sbindir},g' \
		-e 's,@BINDIR@,${bindir},g' \
		-e 's,@SYSCONFDIR@,${sysconfdir},g' \
		-e 's,@BASE_BINDIR@,${base_bindir},g' \
		${D}${systemd_unitdir}/system/checkminerfs.service
}

INSANE_SKIP += "build-deps"

