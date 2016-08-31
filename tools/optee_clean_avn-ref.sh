rm -rf OPTEE_PRE_OPERATION_DONE

bitbake -c cleansstate arm-trusted-firmware l-loader optee-build optee-client optee-os optee-test optee-linuxdriver s5p6818-avn-ref-bl1 s5p6818-avn-ref-uboot
