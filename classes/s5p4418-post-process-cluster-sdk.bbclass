DESCRIPTION = "Nexell Cluster SDK for rootfs"
LICENSE = "CLOSED"

postprocess_install_cluster_sdk_function() {
    #install -d ${IMAGE_ROOTFS}/nexell/cluster/
    #cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/* 	${IMAGE_ROOTFS}/nexell/cluster/
    #install -m0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/TiaPlayer.elf	${IMAGE_ROOTFS}/nexell/cluster/TiaPlayer.elf

    #chmod 755 ${IMAGE_ROOTFS}/nexell/cluster/run.sh
	#cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/script/autorun_cluster.sh       ${IMAGE_ROOTFS}/nexell/
    #chmod 755 ${IMAGE_ROOTFS}/nexell/autorun_cluster.sh

	#install -Dm755  ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/script/cluster.init.d   ${IMAGE_ROOTFS}/etc/init.d/cluster

    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_lib_fb/libMali.so		${IMAGE_ROOTFS}/usr/lib/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_lib_fb/libVR.so			${IMAGE_ROOTFS}/usr/lib/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/camera/libnxavin.so		${IMAGE_ROOTFS}/usr/lib/

    install -d ${IMAGE_ROOTFS}/nexell/camera/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/camera/cluster_camera_720_rgb0	${IMAGE_ROOTFS}/nexell/camera/cluster_camera

	install -d ${IMAGE_ROOTFS}/nexell/fakenavi/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/fakenavi/*	${IMAGE_ROOTFS}/nexell/fakenavi
	chmod 755 ${IMAGE_ROOTFS}/nexell/fakenavi/*

    install -d ${IMAGE_ROOTFS}/nexell/avm_app/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/avm_app/nx_3d_avm   ${IMAGE_ROOTFS}/nexell/avm_app/nx_3d_avm
    chmod 755 ${IMAGE_ROOTFS}/nexell/avm_app/nx_3d_avm

    install -d ${IMAGE_ROOTFS}/sbin/3DS
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/avm_app/bin/3DS/*   ${IMAGE_ROOTFS}/sbin/3DS
    install -d ${IMAGE_ROOTFS}/sbin/avm
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/avm_app/bin/avm/*   ${IMAGE_ROOTFS}/sbin/avm

}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_cluster_sdk_function;"
