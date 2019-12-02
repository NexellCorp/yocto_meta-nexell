### For SWUpdate
DEPENDS += "parted"
FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI_append = " \
    file://sources/handlers \
    "

do_patch[postfuncs] += "do_patch_sources"
do_patch_sources() {
	cp -a ${WORKDIR}/sources/* ${S}
}
