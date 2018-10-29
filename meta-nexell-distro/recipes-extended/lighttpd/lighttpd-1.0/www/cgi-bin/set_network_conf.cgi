#!/bin/sh
#set -x

nexell_conf_nettype=
nexell_conf_hostname=
nexell_conf_ipaddress=
nexell_conf_netmask=
nexell_conf_gateway=
nexell_conf_dnsservers=

nexell_input=`cat /dev/stdin`
#nexell_input=_nexell_conf_nettype=Static\&_nexell_conf_hostname=nexellMiner-A8\&_nexell_conf_ipaddress=192.168.1.3\&_nexell_conf_netmask=255.255.255.0\&_nexell_conf_gateway=192.168.1.1\&_nexell_conf_dnsservers=192.168.1.1
nexell_tmp=${nexell_input//&/ }
i=0
for nexell_var in ${nexell_tmp}
do
	nexell_var=${nexell_var//+/ }
	nexell_var=${nexell_var//%23/#}
	nexell_var=${nexell_var//%24/$}
	nexell_var=${nexell_var//%25/%}
	nexell_var=${nexell_var//%26/&}
	nexell_var=${nexell_var//%2C/,}
	nexell_var=${nexell_var//%2B/+}
	nexell_var=${nexell_var//%3A/:}
	nexell_var=${nexell_var//%3B/;}
	nexell_var=${nexell_var//%3C/<}
	nexell_var=${nexell_var//%3D/=}
	nexell_var=${nexell_var//%3E/>}
	nexell_var=${nexell_var//%3F/?}
	nexell_var=${nexell_var//%40/@}
	nexell_var=${nexell_var//%5B/[}
	nexell_var=${nexell_var//%5D/]}
	nexell_var=${nexell_var//%5E/^}
	nexell_var=${nexell_var//%7B/\{}
	nexell_var=${nexell_var//%7C/|}
	nexell_var=${nexell_var//%7D/\}}
	nexell_var=${nexell_var//%2F/\/}
	#nexell_var=${nexell_var//%22/\"}
	#nexell_var=${nexell_var//%5C/\\}
	case ${i} in
		0 )
		nexell_conf_nettype=${nexell_var/_nexell_conf_nettype=/}
		;;
		1 )
		nexell_conf_hostname=${nexell_var/_nexell_conf_hostname=/}
		;;
		2 )
		nexell_conf_ipaddress=${nexell_var/_nexell_conf_ipaddress=/}
		;;
		3 )
		nexell_conf_netmask=${nexell_var/_nexell_conf_netmask=/}
		;;
		4 )
		nexell_conf_gateway=${nexell_var/_nexell_conf_gateway=/}
		;;
		5 )
		nexell_conf_dnsservers=${nexell_var/_nexell_conf_dnsservers=/}
		;;
	esac
	i=`expr $i + 1`
done

echo "[Match]"							>  /etc/systemd/network/10-eth.network
echo "Name=eth*"						>> /etc/systemd/network/10-eth.network
echo "KernelCommandLine=!root=/dev/nfs"	>> /etc/systemd/network/10-eth.network
echo ""									>> /etc/systemd/network/10-eth.network
echo "[Network]"						>> /etc/systemd/network/10-eth.network
if [ "${nexell_conf_nettype}" == "DHCP" ]; then
	echo "DHCP=yes"						>> /etc/systemd/network/10-eth.network
else
	if [ "${nexell_conf_netmask}" == "255.255.255.0" ]; then
		echo "Address=${nexell_conf_ipaddress}/24"	>> /etc/systemd/network/10-eth.network
	elif [ "${nexell_conf_netmask}" == "255.255.255.192" ]; then
		echo "Address=${nexell_conf_ipaddress}/26"	>> /etc/systemd/network/10-eth.network
	elif [ "${nexell_conf_netmask}" == "255.255.255.240" ]; then
		echo "Address=${nexell_conf_ipaddress}/28"	>> /etc/systemd/network/10-eth.network
	elif [ "${nexell_conf_netmask}" == "255.255.255.252" ]; then
		echo "Address=${nexell_conf_ipaddress}/30"	>> /etc/systemd/network/10-eth.network
	fi
	echo "Gateway=${nexell_conf_gateway}"		>> /etc/systemd/network/10-eth.network
	echo "DNS=${nexell_conf_dnsservers}"	>> /etc/systemd/network/10-eth.network
fi
echo "${nexell_conf_hostname}"			>  /config/hostname
echo "${nexell_conf_hostname}"			>  /etc/hostname
sync
hostname `cat /etc/hostname`

systemctl restart systemd-networkd
/etc/init.d/avahi restart > /dev/null

sleep 3s

echo "ok"
