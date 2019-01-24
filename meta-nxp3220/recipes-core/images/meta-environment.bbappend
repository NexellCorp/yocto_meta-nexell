create_sdk_files() {
    # Setup site file for external use
    toolchain_create_sdk_siteconfig ${SDK_OUTPUT}/${SDKPATH}/site-config-${REAL_MULTIMACH_TARGET_SYS}

    toolchain_create_sdk_env_script ${SDK_OUTPUT}/${SDKPATH}/environment-setup-${REAL_MULTIMACH_TARGET_SYS}

    # Add version information
    toolchain_create_sdk_version ${SDK_OUTPUT}/${SDKPATH}/version-${REAL_MULTIMACH_TARGET_SYS}

    # NEXELL append codes
    echo "export LDFLAGS+=\" --sysroot=\$SDKTARGETSYSROOT \"" >> ${SDK_OUTPUT}/${SDKPATH}/environment-setup-${REAL_MULTIMACH_TARGET_SYS}
    echo "export CFLAGS+=\"${TARGET_CC_ARCH} --sysroot=\$SDKTARGETSYSROOT \"" >> ${SDK_OUTPUT}/${SDKPATH}/environment-setup-${REAL_MULTIMACH_TARGET_SYS}

    echo "export OECORE_SOCNAME=\"${NEXELL_BOARD_SOCNAME}\"" >> ${SDK_OUTPUT}/${SDKPATH}/environment-setup-${REAL_MULTIMACH_TARGET_SYS}
}
