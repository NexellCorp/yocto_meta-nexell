FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI = "file://fstab"

dirs755 += " ${localstatedir}/mmclog "

do_install_append() {
#	ln -snf mmclog ${D}${localstatedir}/log
	install -m 0644 ${WORKDIR}/fstab ${D}${sysconfdir}/fstab
#	echo "/dev/mmcblk0p5	/config			auto	rw,noexec	0 0" >> ${D}${sysconfdir}/fstab
#	echo "/dev/mmcblk0p6	/var/mmclog		auto	rw,noexec	0 0" >> ${D}${sysconfdir}/fstab
}
