FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI += " \
	file://adbd-property_get-ro-secure-disable.patch \
	file://adbd-support-reboot-service.patch \
	file://adbd.mk;subdir=${BPN} \
"

SRC_URI += " \
	file://android-tools-adbd.service \
	file://start_adbd.sh \
	file://stop_adbd.sh \
"

do_compile() {
    # Setting both variables below causing our makefiles to not work with
    # implicit make rules
    unset CFLAGS
    unset CPPFLAGS

    export SRCDIR=${S}

    case "${HOST_ARCH}" in
      arm)
        export android_arch=linux-arm
      ;;
      aarch64)
        export android_arch=linux-arm64
      ;;
      mips|mipsel)
        export android_arch=linux-mips
      ;;
      powerpc|powerpc64)
        export android_arch=linux-ppc
      ;;
      i686|i586|x86_64)
        export android_arch=linux-x86
      ;;
    esac

    for tool in ${TOOLS}; do
      mkdir -p ${B}/${tool}
      oe_runmake -f ${B}/${tool}.mk -C ${B}/${tool}
    done
}

do_install_append() {
    # skip native build system
    if [ -z ${TARGET_PREFIX} ]; then
	return
    fi

    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/android-tools-adbd.service ${D}${systemd_unitdir}/system/

    install -m 0755 ${WORKDIR}/start_adbd.sh ${D}${bindir}/start_adbd.sh
    install -m 0755 ${WORKDIR}/stop_adbd.sh ${D}${bindir}/
}

FILES_${PN} = "${bindir} ${base_sbindir}"
