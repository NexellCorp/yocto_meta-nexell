#SYSTEMD_SERVICE_${PN} = "${@bb.utils.contains('DISTRO_FEATURES', 'use-nxqt-home', '', 'weston.service', d)}"
SYSTEMD_SERVICE_${PN} = "${@bb.utils.contains('DISTRO_FEATURES', 'use-nxqt-home', '', '', d)}"

INSANE_SKIP_${PN} = "installed-vs-shipped"