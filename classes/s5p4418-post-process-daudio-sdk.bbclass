postprocess_install_daudio_sdk_function() {
    install -d ${IMAGE_ROOTFS}/etc/bluetooth
    install -d ${IMAGE_ROOTFS}/usr/bin
    install -d ${IMAGE_ROOTFS}/usr/lib
    install -d ${IMAGE_ROOTFS}/nexell/daudio
    install -d ${IMAGE_ROOTFS}/podo/apps

    cp -aR ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/etc/bluetooth/*	${IMAGE_ROOTFS}/etc/bluetooth/
    cp -aR ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/bin/*		${IMAGE_ROOTFS}/usr/bin/
    cp -aR ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/usr/lib/*		${IMAGE_ROOTFS}/usr/lib/
    cp -aR ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/nexell/daudio/*	${IMAGE_ROOTFS}/nexell/daudio/
    cp -aR ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/podo/apps/*		${IMAGE_ROOTFS}/podo/apps/
    cp -aR ${PWD}/../meta-nexell/recipes-sdk/nexell-daudio-sdk/files/32/podo/podo		${IMAGE_ROOTFS}/podo/
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_daudio_sdk_function;"
