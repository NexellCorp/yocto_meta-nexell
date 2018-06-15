FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-1.0:"

RDEPENDS_${PN} += " \
               lighttpd-module-auth \
               lighttpd-module-authn-file \
               lighttpd-module-cgi \
               lighttpd-module-expire \
"

do_install_append() {
    install -m 0755 ${WORKDIR}/lighttpd.conf ${D}${sysconfdir}
    install -m 0755 ${WORKDIR}/lighttpd-htdigest.user ${D}${sysconfdir}
	cp -pr ${WORKDIR}/www/* ${D}/www/pages
}

SRC_URI_append = " file://lighttpd.conf"
SRC_URI_append = " file://lighttpd-htdigest.user"
SRC_URI_append = " file://www"
