FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI_append = " \
	file://bw_mem.sh \
	"

do_install_append () {
	install -m 0755 ${WORKDIR}/bw_mem.sh ${D}${bindir}/bw_mem.sh
}
