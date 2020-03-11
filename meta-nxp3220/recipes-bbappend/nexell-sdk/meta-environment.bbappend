create_sdk_files_append () {
    script=${SDK_OUTPUT}/${SDKPATH}/environment-setup-${REAL_MULTIMACH_TARGET_SYS}

    echo "" >> $script
    echo "# Append environment for ${MACHINE}" >> $script
    echo "export LDFLAGS+=\" --sysroot=\$SDKTARGETSYSROOT \"" >> $script
    echo "export CFLAGS+=\"${TARGET_CC_ARCH} --sysroot=\$SDKTARGETSYSROOT \"" >> $script
    echo "export OECORE_SOCNAME=${MACHINE}" >> $script
}
