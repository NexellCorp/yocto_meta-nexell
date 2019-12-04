### For SWUpdate services
FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI_append = " \
	file://swupdate.rules \
	file://swupdate-progress \
	"

PACKAGES =+ "${PN}-services"
FILES_${PN}-services = " \
	${sysconfdir}/udev/rules.d/swupdate.rules \
	${sysconfdir}/init.d/swupdate \
	${systemd_unitdir}/systemd/swupdate-progress.service \
	"

# replace do_install in 'swupdate.inc'
do_install () {
	oe_runmake install

	install -m 0755 -d ${D}/www
	if [ -d ${S}/web-app ];then
		cp -R --no-dereference --preserve=mode,links -v ${S}/examples/www/v2/* ${D}/www
	else
		install -m 0755 ${S}/www/* ${D}/www
	fi

	install -d ${D}${sysconfdir}/udev/rules.d
	install -d ${D}${sysconfdir}/init.d

	if [ ! -z "${SWU_SERVICE_SWUPDATE_ROOT}" ]; then
		# Install udev rules
		# if insert or remove external card,
		# the udev execute the /etc/init.d/swupdate script accroding to the swupate.rules
		# NOTE>
		#	swupdte.rules only detect '/dev/sd*' and 'dev/mmcblk1~9'
		install -m 644 ${WORKDIR}/swupdate.rules ${D}${sysconfdir}/udev/rules.d/

		# Install swupdae daemon.
		# excuted by the 'swupdate.rules'. swupdate daemon will be mount external card,
		# and update system with swu image in external card
		install -m 755 ${WORKDIR}/swupdate ${D}${sysconfdir}/init.d
		sed -i -e "s,^EXEC_COMMAND.*,EXEC_COMMAND=\"${SWU_SERVICE_SWUPDATE_ROOT}\",g" \
			${D}${sysconfdir}/init.d/swupdate

		# Install swupdate-progress service for systemd.
		if [ ! -z "${SWU_SERVICE_PROGRESS_ROOT}" ]; then
			install -d ${D}${systemd_system_unitdir}
			install -m 644 ${WORKDIR}/swupdate-progress.service ${D}${systemd_system_unitdir}
			sed -i -e 's,ExecStart.*,ExecStart=${SWU_SERVICE_PROGRESS_ROOT},g' \
				${D}${systemd_system_unitdir}/swupdate-progress.service
		fi
	fi
}

# Remove /etc/rpm-postints, replace to update-rc.d
# refer to update-rc.d.bbclass
PACKAGESPLITFUNCS_remove = "populate_packages_updatercd"

# Remove not necessary services
PACKAGE_EXCLUDE += "swupdate-usb"
RDEPENDS_${PN}-tools_remove = "${PN}-usb"

SYSTEMD_PACKAGES_remove = "${@ '${PN}' if not d.getVar('SWU_SERVICE_SWUPDATE_ROOT', True) else ''}"
SYSTEMD_SERVICE_${PN} = ""
SYSTEMD_SERVICE_${PN}-usb = ""
SYSTEMD_SERVICE_${PN}-progress = "${@ 'swupdate-progress.service' if d.getVar('SWU_SERVICE_SWUPDATE_ROOT', True) and d.getVar('SWU_SERVICE_PROGRESS_ROOT', True) else ''}"
