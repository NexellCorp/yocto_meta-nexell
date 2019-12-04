#
# Copyright (C) 2007 OpenedHand Ltd.
#
# derived from oe-core: meta/recipes-core/packagegroups/packagegroup-core-boot.bb
# add to image:
# IMAGE_INSTALL_append = "packagegroup-<name>"

SUMMARY = "SWUpdate requirements"
DESCRIPTION = "The swupdate packages set"
LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGE_SWUPDATE = " \
	swupdate \
	swupdate-tools \
	udev \
	"

PACKAGE_SWUPDATE += " ${@ 'swupdate-services' if d.getVar('SWU_SERVICE_SWUPDATE_ROOT', True) else ''} "
PACKAGE_SWUPDATE += " ${@ 'psplash' if d.getVar('SWU_PROGRESS_PSPLASH') else ''} "

RDEPENDS_${PN} = " \
	${@bb.utils.contains('DISTRO_FEATURES', 'swupdate', '${PACKAGE_SWUPDATE}', '', d)} \
	"
