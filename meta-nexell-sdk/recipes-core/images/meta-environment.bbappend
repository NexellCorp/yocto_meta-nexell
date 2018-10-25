create_sdk_files() {
    # Setup site file for external use
    toolchain_create_sdk_siteconfig ${SDK_OUTPUT}/${SDKPATH}/site-config-${REAL_MULTIMACH_TARGET_SYS}

    toolchain_create_sdk_env_script ${SDK_OUTPUT}/${SDKPATH}/environment-setup-${REAL_MULTIMACH_TARGET_SYS}

    # Add version information
    toolchain_create_sdk_version ${SDK_OUTPUT}/${SDKPATH}/version-${REAL_MULTIMACH_TARGET_SYS}

    # NEXELL append codes
    echo "export LDFLAGS+=\" --sysroot=\$SDKTARGETSYSROOT \"" >> ${SDK_OUTPUT}/${SDKPATH}/environment-setup-${REAL_MULTIMACH_TARGET_SYS}
    echo "export CFLAGS+=\"${TARGET_CC_ARCH} --sysroot=\$SDKTARGETSYSROOT \"" >> ${SDK_OUTPUT}/${SDKPATH}/environment-setup-${REAL_MULTIMACH_TARGET_SYS}
}

# STAGING_KERNEL_DIR => kernel source path
# SDPATH => /opt/poky/2.5.1
# REAL_MULTIMACH_TARGET_SYS => cortexa9hf-neon-poky-linux-gnueabi
do_install_append() {
    install -d ${D}/${SDKPATH}/sysroots/${REAL_MULTIMACH_TARGET_SYS}/usr/include/uapi/.
    install -m 0644 ${STAGING_KERNEL_DIR}/include/uapi/linux/videodev2_nxp_media.h ${D}/${SDKPATH}/sysroots/${REAL_MULTIMACH_TARGET_SYS}/usr/include/uapi/.

    install -d ${D}/${SDKPATH}/sysroots/${REAL_MULTIMACH_TARGET_SYS}/usr/include/KHR
    install -m 0644 ${BASE_WORKDIR}/for_sdk/usr/include/KHR/khrplatform.h ${D}/${SDKPATH}/sysroots/${REAL_MULTIMACH_TARGET_SYS}/usr/include/KHR/.
}
