### For SWUpdate
RDEPENDS_${PN} = "parted"

FILESEXTRAPATHS_append := ":${THISDIR}/files"
SRC_URI_append = " \
	file://sources/handlers \
	file://swupdate.rules \
	"

do_patch[postfuncs] += "do_patch_sources"
do_patch_sources() {
	cp -a ${WORKDIR}/sources/* ${S}
}

# Replace swupdate services
require swupdate-services.inc
