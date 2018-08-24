DESCRIPTION = "Nexell Cluster SDK for rootfs"
LICENSE = "CLOSED"

postprocess_install_cluster_sdk_function() {
    install -d ${IMAGE_ROOTFS}/nexell/cluster/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/* 	${IMAGE_ROOTFS}/nexell/cluster/
    #cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/libSDL2-2.0.so.0.2.1		${IMAGE_ROOTFS}/usr/lib/
    #cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/libSDL2_ttf.so			${IMAGE_ROOTFS}/usr/lib/
	#ln -s -r ${IMAGE_ROOTFS}/usr/lib/libSDL2-2.0.so.0.2.1 ${IMAGE_ROOTFS}/usr/lib/libSDL2-2.0.so.0
	#ln -s -r ${IMAGE_ROOTFS}/usr/lib/libSDL2-2.0.so.0.2.1 ${IMAGE_ROOTFS}/usr/lib/libSDL2.so
    install -m0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/TiaPlayer.elf	${IMAGE_ROOTFS}/nexell/cluster/TiaPlayer.elf

    #cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/script/autorun_cluster.sh	${IMAGE_ROOTFS}/etc/profile.d/autorun_cluster.sh
    chmod 755 ${IMAGE_ROOTFS}/nexell/cluster/run.sh
    #chmod 755 ${IMAGE_ROOTFS}/etc/profile.d/autorun_cluster.sh
	#install -Dm0644 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/script/cluster.service  ${IMAGE_ROOTFS}/etc/systemd/system/cluster.service
	#install -Dm0644 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/script/cluster.service  ${IMAGE_ROOTFS}${systemd_system_unitdir}/
	install -Dm755  ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/script/cluster.init.d   ${IMAGE_ROOTFS}/etc/init.d/cluster

    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_lib_fb/libMali.so		${IMAGE_ROOTFS}/usr/lib/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_lib_fb/libVR.so			${IMAGE_ROOTFS}/usr/lib/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/camera/libnxavin.so		${IMAGE_ROOTFS}/usr/lib/

    install -d ${IMAGE_ROOTFS}/nexell/camera/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/camera/cluster_camera_720_rgb0	${IMAGE_ROOTFS}/nexell/camera/cluster_camera

	install -d ${IMAGE_ROOTFS}/nexell/fakenavi/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/fakenavi/*	${IMAGE_ROOTFS}/nexell/fakenavi
	chmod 755 ${IMAGE_ROOTFS}/nexell/fakenavi/*

}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_cluster_sdk_function;"
