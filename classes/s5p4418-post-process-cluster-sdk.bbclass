DESCRIPTION = "Nexell Cluster SDK for rootfs"
LICENSE = "CLOSED"

postprocess_install_cluster_sdk_function() {
    install -d ${IMAGE_ROOTFS}/nexell/cluster/
    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/* 	${IMAGE_ROOTFS}/nexell/cluster/
    chmod 755 ${IMAGE_ROOTFS}/nexell/cluster/run.sh
    install -m0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_ui/TiaPlayer.elf	${IMAGE_ROOTFS}/nexell/cluster/TiaPlayer.elf

    if test -e ${IMAGE_ROOTFS}/etc/init.d/cluster ; then
        rm ${IMAGE_ROOTFS}/etc/init.d/cluster
    fi
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

    install -Dm0644 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/cluster.service	${IMAGE_ROOTFS}/etc/systemd/system/cluster.service
    install -Dm755  ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/cluster.init.d	${IMAGE_ROOTFS}/etc/init.d/cluster
    #ln -sf ../init.d/cluster ${IMAGE_ROOTFS}/etc/rc0.d/K19cluster
    #ln -sf ../init.d/cluster ${IMAGE_ROOTFS}/etc/rc1.d/K19cluster
    #ln -sf ../init.d/cluster ${IMAGE_ROOTFS}/etc/rc2.d/S10cluster
    #ln -sf ../init.d/cluster ${IMAGE_ROOTFS}/etc/rc5.d/S10cluster
    #ln -sf ../init.d/cluster ${IMAGE_ROOTFS}/etc/rc6.d/K19cluster

    install -Dm755 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/weston.init.d		${IMAGE_ROOTFS}/etc/init.d/weston
    install -Dm755 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/weston.ini			${IMAGE_ROOTFS}/etc/weston.ini
    #install -Dm0644 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/weston.service	${IMAGE_ROOTFS}/etc/systemd/system/weston.service

    # Install weston-start script
    install -Dm755 ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/weston-start		${IMAGE_ROOTFS}/usr/bin/weston-start
    sed -i 's,@DATADIR@,${datadir},g'																			${IMAGE_ROOTFS}/usr/bin/weston-start
    sed -i 's,@LOCALSTATEDIR@,${localstatedir},g'																${IMAGE_ROOTFS}/usr/bin/weston-start

    cp -aRfv ${PWD}/../meta-nexell/recipes-sdk/nexell-cluster-sdk/files/cluster_weston/logo/Nexelllogo.png		${IMAGE_ROOTFS}/usr/share/weston/Nexelllogo.png
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_cluster_sdk_function;"
