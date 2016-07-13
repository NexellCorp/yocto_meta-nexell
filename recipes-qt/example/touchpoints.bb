DESCRIPTION = "MultiTouch Test app"
LICENSE = "LGPLv2.1"

inherit systemd

SRC_URI = " \
	file://touchpoints.qml \
	file://touchpoints.service \
	"

do_configure[noexec] = "1"
do_compile[noexec] = "1"

COMPATIBLE_MACHINE = "(s5p6818-artik710-raptor|s5p4418-avn-ref|s5p4418-navi-ref)"

do_install() {
	install -Dm0644 ${WORKDIR}/touchpoints.qml ${D}/opt/example/touchpoints.qml
	install -Dm0644 ${WORKDIR}/touchpoints.service ${D}/${systemd_unitdir}/system/touchpoints.service
}

FILES_${PN} += " \
	/opt/example/ \
	/lib/systemd/ \
	"

RDEPENDS_${PN} += "qtdeclarative-tools"
