SUMMARY = "Nexell userland spidev test application"
LICENSE = "CLOSED"

SRC_URI = "file://spidev_test.c \
		   file://runbist1ch.sh \
		   file://runjob1ch.sh \
		   file://runbist2ch.sh \
		   file://runjob2ch.sh \
		   file://runbist4ch.sh \
		   file://runjob4ch.sh \
		   file://runspireset.sh \
	"

S = "${WORKDIR}"

#SRCREV = "${AUTOREV}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile() {
	${CC} spidev_test.c -o spidevtest
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/spidevtest ${D}${bindir}
	install -m 0755 ${WORKDIR}/runbist1ch.sh ${D}${bindir}
	install -m 0755 ${WORKDIR}/runjob1ch.sh ${D}${bindir}
	install -m 0755 ${WORKDIR}/runbist2ch.sh ${D}${bindir}
	install -m 0755 ${WORKDIR}/runjob2ch.sh ${D}${bindir}
	install -m 0755 ${WORKDIR}/runbist4ch.sh ${D}${bindir}
	install -m 0755 ${WORKDIR}/runjob4ch.sh ${D}${bindir}
	install -m 0755 ${WORKDIR}/runspireset.sh ${D}${bindir}
}

FILES_${PN} = "${bindir}"
INSANE_SKIP_${PN} = "ldflags"
