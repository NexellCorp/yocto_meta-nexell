do_configure() {
	unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS

	oe_runmake CC="${KERNEL_CC}" LD="${KERNEL_LD}" AR="${KERNEL_AR}" \
	           -C ${BASE_WORKDIR}/clone_kernel_src O=${STAGING_KERNEL_BUILDDIR} scripts prepare
}
