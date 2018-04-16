DESCRIPTION = "Nexell Cluster SDK for rootfs"
LICENSE = "CLOSED"

postprocess_install_cluster_sdk_function() {
    install -d ${IMAGE_ROOTFS}/nexell/cluster/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/* 	${IMAGE_ROOTFS}/nexell/cluster/
    chmod 755 ${IMAGE_ROOTFS}/nexell/cluster/run.sh
    install -m0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/TiaPlayer.elf	${IMAGE_ROOTFS}/nexell/cluster/TiaPlayer.elf

    if test -e ${IMAGE_ROOTFS}/etc/init.d/weston ; then
        rm ${IMAGE_ROOTFS}/etc/init.d/weston
    fi
    if test -e ${IMAGE_ROOTFS}/home/root/.config/weston.ini ; then
        rm ${IMAGE_ROOTFS}/home/root/.config/weston.ini
    fi
    if test -e ${IMAGE_ROOTFS}/etc/systemd/system/weston.service ; then
        rm ${IMAGE_ROOTFS}/etc/systemd/system/weston.service
    fi
    if test -e ${IMAGE_ROOTFS}/usr/bin/weston-start ; then
        rm ${IMAGE_ROOTFS}/usr/bin/weston-start
    fi

    install -Dm755 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/init				${IMAGE_ROOTFS}/etc/init.d/weston
    install -Dm755 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/weston.ini			${IMAGE_ROOTFS}/home/root/.config/weston.ini
    #install -Dm0644 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/weston.service	${IMAGE_ROOTFS}/etc/systemd/system/weston.service

    # Install weston-start script
    install -Dm755 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/weston-start		${IMAGE_ROOTFS}/usr/bin/weston-start
    sed -i 's,@DATADIR@,${datadir},g'																			${IMAGE_ROOTFS}/usr/bin/weston-start
    sed -i 's,@LOCALSTATEDIR@,${localstatedir},g'																${IMAGE_ROOTFS}/usr/bin/weston-start
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_cluster_sdk_function;"
