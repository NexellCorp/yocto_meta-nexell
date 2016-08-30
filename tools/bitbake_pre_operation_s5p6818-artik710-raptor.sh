echo -e "\n\033[47;34m ------------------------------------------------------------------ \033[0m"
echo -e "\033[47;34m   ATF & l-loader & optee & u-boot & bl1 ==> do_fetch               \033[0m"
echo -e "\033[47;34m ------------------------------------------------------------------ \033[0m"
bitbake arm-trusted-firmware l-loader optee-client optee-os optee-test optee-linuxdriver s5p6818-artik710-raptor-bl1 s5p6818-artik710-raptor-uboot -c fetch

echo -e "\n\033[47;34m ------------------------------------------------------------------ \033[0m"
echo -e "\033[47;34m  ATF & l-loader & optee & u-boot & bl1 ==> do_unpack               \033[0m"
echo -e "\033[47;34m ------------------------------------------------------------------ \033[0m"
bitbake arm-trusted-firmware l-loader optee-client optee-os optee-test optee-linuxdriver s5p6818-artik710-raptor-bl1 s5p6818-artik710-raptor-uboot -c unpack

echo -e "\033[47;34m ------------------------------------------------------------------ \033[0m"
echo -e "\033[47;34m            do_fetch & do_unpack Done...                            \033[0m"
echo -e "\033[47;34m ------------------------------------------------------------------ \033[0m"
