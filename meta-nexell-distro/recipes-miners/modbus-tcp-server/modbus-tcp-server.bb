SUMMARY = "Nexell modbus tcp server for miner"
LICENSE = "CLOSED"

DEPENDS = "libmodbus"

SRC_URI = "file://* \
	"

S = "${WORKDIR}"

inherit pkgconfig

#SRCREV = "${AUTOREV}"

#PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
#	${CC} modbus-tcp-server.c ${CFLAGS} ${LDFLAGS} `pkg-config --cflags --libs libmodbus` -o modbus-tcp-server
	oe_runmake
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 modbus-tcp-server ${D}${bindir}
}

#PACKAGES = "${PN}"
#FILES_${PN} = "${bindir} "
