DESCRIPTION = "Nexell convergence-svmc for rootfs"
LICENSE = "CLOSED"

CONVERGENCE_SVMC_PLATFORM ?= "${@bb.utils.contains('DISTRO_FEATURES', 'nexell-convergence-svmc', 'YES', 'NO', d)}"
CONVERGENCE_SVMC_PATH = "../../meta-nexell/meta-nexell-distro/recipes-application/convergence-svmc/files/convergence-svmc"

postprocess_install_convergence_svmc_function() {
    if [ "${CONVERGENCE_SVMC_PLATFORM}" = "YES" ]; then
        echo "CONVERGENCE_SVMC_PLATFORM YES"

        install -d ${IMAGE_ROOTFS}/nexell/
        install -d ${IMAGE_ROOTFS}/nexell/convergence-svmc
        install -d ${IMAGE_ROOTFS}/nexell/camera
        install -d ${IMAGE_ROOTFS}/nexell/fakenavi
        install -d ${IMAGE_ROOTFS}/nexell/avm_app
        install -d ${IMAGE_ROOTFS}/sbin
        install -d ${IMAGE_ROOTFS}/sbin/3DS
        install -d ${IMAGE_ROOTFS}/sbin/avm
        install -d ${IMAGE_ROOTFS}/usr
        install -d ${IMAGE_ROOTFS}/usr/lib
        install -d ${IMAGE_ROOTFS}/etc
        install -d ${IMAGE_ROOTFS}/etc/init.d

        cp -aRfv ${PWD}/${CONVERGENCE_SVMC_PATH}/convergence-svmc_ui/* 	${IMAGE_ROOTFS}/nexell/convergence-svmc/
#        install -m 0755 --owner=root --group=root -s --strip-program=${STRIP} ${PWD}/${CONVERGENCE_SVMC_PATH}/convergence-svmc_ui/TiaPlayer.elf	${IMAGE_ROOTFS}/nexell/convergence-svmc/TiaPlayer.elf
        install -m 0755 --owner=root --group=root ${PWD}/${CONVERGENCE_SVMC_PATH}/convergence-svmc_ui/TiaPlayer.elf	${IMAGE_ROOTFS}/nexell/convergence-svmc/TiaPlayer.elf

        chmod 755 ${IMAGE_ROOTFS}/nexell/convergence-svmc/run.sh
    	cp -aRfv ${PWD}/${CONVERGENCE_SVMC_PATH}/script/autorun_convergence-svmc.sh       ${IMAGE_ROOTFS}/nexell/
        chmod 755 ${IMAGE_ROOTFS}/nexell/autorun_convergence-svmc.sh

    	install -Dm 0755  ${PWD}/${CONVERGENCE_SVMC_PATH}/script/convergence-svmc.init.d   ${IMAGE_ROOTFS}/etc/init.d/convergence-svmc

        cp -aRfv ${PWD}/${CONVERGENCE_SVMC_PATH}/convergence-svmc_lib_fb/libMali.so		${IMAGE_ROOTFS}/usr/lib/
        cp -aRfv ${PWD}/${CONVERGENCE_SVMC_PATH}/convergence-svmc_lib_fb/libVR.so			${IMAGE_ROOTFS}/usr/lib/
        cp -aRfv ${PWD}/${CONVERGENCE_SVMC_PATH}/camera/libnxavin.so		${IMAGE_ROOTFS}/usr/lib/
        cp -aRfv ${PWD}/${CONVERGENCE_SVMC_PATH}/camera/convergence-svmc_camera_720_rgb0	${IMAGE_ROOTFS}/nexell/camera/convergence-svmc_camera

        cp -aRfv ${PWD}/${CONVERGENCE_SVMC_PATH}/fakenavi/*	${IMAGE_ROOTFS}/nexell/fakenavi
    	chmod 0755 ${IMAGE_ROOTFS}/nexell/fakenavi/*

        cp -aRfv ${PWD}/${CONVERGENCE_SVMC_PATH}/avm_app/nx_3d_avm   ${IMAGE_ROOTFS}/nexell/avm_app/nx_3d_avm
        chmod 0755 ${IMAGE_ROOTFS}/nexell/avm_app/nx_3d_avm

        cp -aRfv ${PWD}/${CONVERGENCE_SVMC_PATH}/avm_app/bin/3DS/*   ${IMAGE_ROOTFS}/sbin/3DS
        cp -aRfv ${PWD}/${CONVERGENCE_SVMC_PATH}/avm_app/bin/avm/*   ${IMAGE_ROOTFS}/sbin/avm
    else
        echo "CONVERGENCE_SVMC_PLATFORM NO"
        echo "DO NOTHING"
    fi
}

ROOTFS_POSTPROCESS_COMMAND += "postprocess_install_convergence_svmc_function;"
