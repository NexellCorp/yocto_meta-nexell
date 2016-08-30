echo 'OPTEE_DO_PRE_TASKS NOK' > ../meta-nexell/tools/optee_status.cfg

bitbake -c cleansstate arm-trusted-firmware l-loader optee-build optee-client optee-os optee-test optee-linuxdriver s5p6818-avn-ref-bl1 s5p6818-avn-ref-uboot
