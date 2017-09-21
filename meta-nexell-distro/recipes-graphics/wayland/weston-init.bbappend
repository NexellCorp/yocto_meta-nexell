#Override for do not use weston desktop shell

do_install() {
         install -Dm755 ${WORKDIR}/init ${D}/${sysconfdir}/init.d/weston
         #install -Dm0644 ${WORKDIR}/weston.service ${D}${systemd_system_unitdir}/weston.service

         # Install weston-start script
         install -Dm755 ${WORKDIR}/weston-start ${D}${bindir}/weston-start
         sed -i 's,@DATADIR@,${datadir},g' ${D}${bindir}/weston-start
         sed -i 's,@LOCALSTATEDIR@,${localstatedir},g' ${D}${bindir}/weston-start
}

SYSTEMD_SERVICE_${PN} = ""