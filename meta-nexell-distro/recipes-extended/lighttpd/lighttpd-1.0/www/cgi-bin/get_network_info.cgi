#!/bin/sh

echo {

# Read network configuration
nexell_nettype=Static
nexell_netdevice=
nexell_macaddr=
nexell_ipaddress=
nexell_netmask=

if [ -s /config/network.conf ] ; then
	nexell_nettype=`cat /config/network.conf | grep dhcp`
	if [ -z nexell_nettype ]; then
		nexell_nettype="dhcp=false"
	fi
	nexell_nettype=${nexell_nettype/dhcp=/}
	if [ "${nexell_nettype}" == "true" ]; then
		nexell_nettype=DHCP
	else
		nexell_nettype=Static
	fi
fi

nexell_tmp=`ifconfig | grep eth`
i=0
for nexell_var in ${nexell_tmp}
do
	case ${i} in
		0 )
		nexell_netdevice=${nexell_var}
		;;
		4 )
		nexell_macaddr=${nexell_var}
		;;
	esac
	i=`expr $i + 1`
done

nexell_tmp=`ifconfig | grep "inet addr"`
i=0
for nexell_var in ${nexell_tmp}
do
	case ${i} in
		1 )
		nexell_ipaddress=${nexell_var}
		nexell_ipaddress=${nexell_ipaddress/addr:/}
		;;
		3 )
		nexell_netmask=${nexell_var}
		nexell_netmask=${nexell_netmask/Mask:/}
		;;
	esac
	i=`expr $i + 1`
done

echo \"nettype\":\"${nexell_nettype}\",
echo \"netdevice\":\"${nexell_netdevice}\",
echo \"macaddr\":\"${nexell_macaddr}\",
echo \"ipaddress\":\"${nexell_ipaddress}\",
echo \"netmask\":\"${nexell_netmask}\",

nexell_conf_nettype=${nexell_nettype}
nexell_conf_hostname=
nexell_conf_ipaddress=
nexell_conf_netmask=
nexell_conf_gateway=
nexell_conf_dnsservers=

if [ -s /config/network.conf ] ; then	
	nexell_conf_hostname=`cat /config/network.conf | grep hostname`
	nexell_conf_hostname=${nexell_conf_hostname/hostname=/}
	nexell_conf_ipaddress=`cat /config/network.conf | grep ipaddress`
	nexell_conf_ipaddress=${nexell_conf_ipaddress/ipaddress=/}
	nexell_conf_netmask=`cat /config/network.conf | grep netmask`
	nexell_conf_netmask=${nexell_conf_netmask/netmask=/}
	nexell_conf_gateway=`cat /config/network.conf | grep gateway`
	nexell_conf_gateway=${nexell_conf_gateway/gateway=/}
	nexell_conf_dnsservers=`cat /config/network.conf | grep dnsservers`
	nexell_conf_dnsservers=${nexell_conf_dnsservers/dnsservers=/}
	nexell_conf_dnsservers=${nexell_conf_dnsservers//\"/}
fi

echo \"conf_nettype\":\"${nexell_conf_nettype}\",
echo \"conf_hostname\":\"${nexell_conf_hostname}\",
echo \"conf_ipaddress\":\"${nexell_conf_ipaddress}\",
echo \"conf_netmask\":\"${nexell_conf_netmask}\",
echo \"conf_gateway\":\"${nexell_conf_gateway}\",
echo \"conf_dnsservers\":\"${nexell_conf_dnsservers}\"

echo }
