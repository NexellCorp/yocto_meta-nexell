DESCRIPTION = "Nexell Cluster SDK for rootfs"
LICENSE = "CLOSED"

postprocess_install_cluster_sdk_function() {
    install -d ${IMAGE_ROOTFS}/nexell/cluster/

    cp -aRf ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/* 	${IMAGE_ROOTFS}/nexell/cluster/

    chmod 755 ${IMAGE_ROOTFS}/nexell/cluster/run.sh

    install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/TiaPlayer.elf	${IMAGE_ROOTFS}/nexell/cluster/TiaPlayer.elf

}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_cluster_sdk_function;"
