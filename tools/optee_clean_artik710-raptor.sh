rm -rf OPTEE_PRE_OPERATION_DONE

bitbake -c cleansstate arm-trusted-firmware l-loader optee-build optee-client optee-os optee-test optee-linuxdriver s5p6818-artik710-raptor-bl1 s5p6818-artik710-raptor-uboot
