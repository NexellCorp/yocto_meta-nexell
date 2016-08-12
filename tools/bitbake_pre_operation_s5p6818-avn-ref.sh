echo -e "\033[40;33m ATF & l-loader & optee & u-boot & bl1 ==> pre fetch & unpacking \033[0m"
bitbake arm-trusted-firmware l-loader optee-client optee-os optee-test optee-linuxdriver s5p6818-avn-ref-bl1 s5p6818-avn-ref-uboot -c fetch
bitbake arm-trusted-firmware l-loader optee-client optee-os optee-test optee-linuxdriver s5p6818-avn-ref-bl1 s5p6818-avn-ref-uboot -c unpack
echo -e "\033[40;33m fetch & unpacking Done... \033[0m"
