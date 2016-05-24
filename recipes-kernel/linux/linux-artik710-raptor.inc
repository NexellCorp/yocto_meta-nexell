require linux-nexell.inc
inherit linux-nexell-base

DESCRIPTION = "Linux Kernel for nexell artik710-raptor"
SECTION = "kernel"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

SRC_URI += " \
        file://defconfig \
        "

COMPATIBLE_MACHINE = "artik710-raptor"
DEFAULTTUNE ?= "aarch64"

PE = "1"
PV = "4.1.0322"

KERNEL_DEFCONFIG_artik710-raptor ?= "artik710_raptor_defconfig"

UDEV_GE_141 ?= "1"

KERNEL_MODULE_AUTOLOAD += "${@bb.utils.contains("MACHINE_FEATURES", "pitft28r", "stmpe-ts", "", d)}"

python __anonymous () {
    kerneltype = d.getVar('KERNEL_IMAGETYPE', True)
    kerneldt = get_dts(d, d.getVar('LINUX_VERSION', True))
    d.setVar("KERNEL_DEVICETREE", kerneldt)
}

do_kernel_configme_prepend() {
    config=${S}/arch/arm64/configs/${KERNEL_DEFCONFIG}
    sed -i "/CONFIG_SMP[ =]/d" $config
    echo "CONFIG_SMP=n" >> $config
    sed -i "/CONFIG_CMDLINE[ =]/d" $config
    
    install -m 0644 ${S}/arch/arm64/configs/${KERNEL_DEFCONFIG} ${WORKDIR}/defconfig || die "No default configuration for ${MACHINE} / ${KERNEL_DEFCONFIG} available."
}

do_boot_mkimage() {
    if test "x${KERNEL_IMAGETYPE}" != "xuImage" ; then
        if test -n "${KERNEL_DEVICETREE}"; then
            # Add RPi bootloader trailer to kernel image to enable DeviceTree support
            ${STAGING_BINDIR_NATIVE}/mkknlimg --dtok ${KERNEL_OUTPUT} ${KERNEL_OUTPUT}
        fi
    fi
}

do_compile() {
    oe_runmake ARCH=arm64 distclean
    local dts_file=${S}/arch/arm64/boot/dts/nexell/s5p6818-artik710.dtsi

    echo -e "\033[44;33m << ----debug suker---- KERNEL_DEFCONFIG ==> ${KERNEL_DEFCONFIG} >>\033[0m"
    oe_runmake ARCH=arm64 artik710_raptor_defconfig

#   TARGET_LDFLAGS="-Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed"   
    export LDFLAGS="-O1 --hash-style=gnu --as-needed"

    local memreserve_size=0x7ed00000
    local reg_size=0x3ee00000

    echo "memreserve_size --> ${memreserve_size}"
    echo "reg_size --> ${reg_size}"

    sed -i -e 's/\(\/memreserve\/[[:blank:]]\+\)\([[:alnum:]]\+[[:blank:]]\+\)\([[:alnum:]]\+\)/\1'"${memreserve_size}"' \3/' ${dts_file} 
    sed -i -e 's/\(^[[:blank:]]\+reg = <0x40000000 \)[[:alnum:]]\+>/\1'"${reg_size}"'>/' ${dts_file}   

    oe_runmake ARCH=arm64 CROSS_COMPILE=aarch64-poky-linux- Image -j8
    oe_runmake ARCH=arm64 CROSS_COMPILE=aarch64-poky-linux- dtbs
    oe_runmake ARCH=arm64 CROSS_COMPILE=aarch64-poky-linux- modules -j8
}

addtask boot_mkimage before do_install after do_compile