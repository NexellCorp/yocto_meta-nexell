#echo -e "\033[40;33m ATF & l-loader & optee & u-boot & bl1 ==> pre fetch & unpacking \033[0m"
#bitbake arm-trusted-firmware l-loader optee-client optee-os optee-test optee-linuxdriver bl1-avn u-boot-artik7 -c fetch
#bitbake arm-trusted-firmware l-loader optee-client optee-os optee-test optee-linuxdriver bl1-avn u-boot-artik7 -c unpack
#echo -e "\033[40;33m fetch & unpacking Done... \033[0m"
#echo -e "\033[40;33m You are now ready to run the bitbake command for NEXELL\033[0m"
#echo -e "\033[40;33m                                                        \033[0m"
#echo -e "\033[40;33m   ex) $ bitbake nexell-artik710-raptor-tiny            \033[0m"
#echo -e "\033[40;33m   ex) $ bitbake nexell-artik710-raptor-sato            \033[0m"
#echo -e "\033[40;33m   ex) $ bitbake nexell-artik710-raptor-qt              \033[0m"

echo -e "\033[40;33m AVN 4418 board not use optee & ATF \033[0m"
