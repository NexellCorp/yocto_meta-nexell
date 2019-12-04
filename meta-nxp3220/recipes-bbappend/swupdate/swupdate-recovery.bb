### For SWUpdate
LICENSE = "CLOSED"
RDEPENDS_${PN} = "swupdate"

# for install files to recovery root filesystem
FILESEXTRAPATHS_append := ":${THISDIR}/files"
SRC_URI = " \
	file://swupdate.rules \
	file://swupdate \
	file://swupdate-progress \
	"

SRC_URI_append = " \
	file://${SWU_TOOL_IMAGE_GEN} \
	file://${SWU_TOOL_HASH_GEN} \
	"

# remove package 'swupdate-services'
# this macro remove FILES_swupdate-sevice package in swupdate_%.bbappend
PACKAGE_EXCLUDE += "swupdate-sercices"

do_install () {
	install -d ${D}${sysconfdir}/udev/rules.d
	install -d ${D}${sysconfdir}/init.d

	if [ ! -z "${SWU_SERVICE_SWUPDATE_RECOVERY}" ]; then
		# Install udev rules
		# if insert or remove external card,
		# the udev execute the /etc/init.d/swupdate script accroding to the swupate.rules
		# NOTE>
		#	swupdate.rules only detect '/dev/sd*' and 'dev/mmcblk1~9'
		install -m 644 ${WORKDIR}/swupdate.rules ${D}${sysconfdir}/udev/rules.d/

		# Install swupdae daemon.
		# excuted by the 'swupdate.rules'. swupdate daemon will be mount external card,
		# and update system with swu image in external card
		install -m 755 ${WORKDIR}/swupdate ${D}${sysconfdir}/init.d
		sed -i -e "s,^EXEC_COMMAND.*,EXEC_COMMAND=\"${SWU_SERVICE_SWUPDATE_RECOVERY}\",g" \
			${D}${sysconfdir}/init.d/swupdate

		# Install swupdate-progress service for sysV.
		if [ ! -z "${SWU_SERVICE_PROGRESS_RECOVERY}" ]; then
			install -m 755 ${WORKDIR}/swupdate-progress ${D}${sysconfdir}/init.d
			sed -i -e 's,^EXEC_COMMAND.*,EXEC_COMMAND=\"${SWU_SERVICE_PROGRESS_RECOVERY}\",g' \
				${D}${sysconfdir}/init.d/swupdate-progress
		fi
	fi
}

do_deploy() {
	install -m 0755 ${WORKDIR}/${SWU_TOOL_IMAGE_GEN} ${DEPLOY_DIR_IMAGE}
	install -m 0755 ${WORKDIR}/${SWU_TOOL_HASH_GEN} ${DEPLOY_DIR_IMAGE}
}
addtask deploy before do_build after do_compile

#
# Set auto-run with update-rc.d
#
inherit update-rc.d

# Should be higher priority than "/etc/rcS/S03systemd-udevd"
# and lower priority than "/etc/rcS.d/S02sysfs.sh"
INITSCRIPT_NAME = "${@ 'swupdate-progress' if d.getVar('SWU_SERVICE_SWUPDATE_RECOVERY', True) and  d.getVar('SWU_SERVICE_PROGRESS_RECOVERY', True)  else ''}"
INITSCRIPT_PARAMS = "start 03 S ."

# Skip update-rc.d refer to update-rc.d.bbclass
PACKAGESPLITFUNCS_remove = "${@ 'populate_packages_updatercd' if not d.getVar('SWU_SERVICE_SWUPDATE_RECOVERY', True) or not d.getVar('SWU_SERVICE_PROGRESS_RECOVERY', True) else ''}"
