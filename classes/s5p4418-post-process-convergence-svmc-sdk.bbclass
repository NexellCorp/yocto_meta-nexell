DESCRIPTION = "Nexell convergence-svmc SDK for rootfs"
LICENSE = "CLOSED"

postprocess_install_convergence_svmc_sdk_function() {

    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/linux/convergence-svmc-sdk/files/convergence-svmc_lib_fb/libMali.so		${IMAGE_ROOTFS}/usr/lib/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/linux/convergence-svmc-sdk/files/convergence-svmc_lib_fb/libVR.so			${IMAGE_ROOTFS}/usr/lib/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/linux/convergence-svmc-sdk/files/camera/libnxavin.so		${IMAGE_ROOTFS}/usr/lib/

    install -d ${IMAGE_ROOTFS}/nexell/camera/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/linux/convergence-svmc-sdk/files/camera/convergence-svmc_camera_720_rgb0	${IMAGE_ROOTFS}/nexell/camera/convergence-svmc_camera

	install -d ${IMAGE_ROOTFS}/nexell/fakenavi/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/linux/convergence-svmc-sdk/files/fakenavi/*	${IMAGE_ROOTFS}/nexell/fakenavi
	chmod 755 ${IMAGE_ROOTFS}/nexell/fakenavi/*

    install -d ${IMAGE_ROOTFS}/nexell/avm_app/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/linux/convergence-svmc-sdk/files/avm_app/nx_3d_avm   ${IMAGE_ROOTFS}/nexell/avm_app/nx_3d_avm
    chmod 755 ${IMAGE_ROOTFS}/nexell/avm_app/nx_3d_avm

    install -d ${IMAGE_ROOTFS}/sbin/3DS
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/linux/convergence-svmc-sdk/files/avm_app/bin/3DS/*   ${IMAGE_ROOTFS}/sbin/3DS
    install -d ${IMAGE_ROOTFS}/sbin/avm
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/linux/convergence-svmc-sdk/files/avm_app/bin/avm/*   ${IMAGE_ROOTFS}/sbin/avm

}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_convergence_svmc_sdk_function;"
