DESCRIPTION = "ubuntu extra support images"
LICENSE = "CLOSED"
SECTION = "libs"

SRC_URI = " \
 file://etc \
 file://usr \
"

S = "${WORKDIR}"

do_install () {
    install -d ${D}/etc
    install -d ${D}/etc/init
    install -d ${D}/usr/bin

    #install -m 0644 ${S}/etc/init/failsafe.conf ${D}/etc/init/
    install -m 0644 ${S}/etc/init/ttyAMA3.conf  ${D}/etc/init/
    install -m 0644 ${S}/etc/rc.local           ${D}/etc/
    install -m 0644 ${S}/usr/bin                ${D}/usr/bin/
}                                                                               

FILES_${PN} = " /etc /etc/init /usr/bin"
