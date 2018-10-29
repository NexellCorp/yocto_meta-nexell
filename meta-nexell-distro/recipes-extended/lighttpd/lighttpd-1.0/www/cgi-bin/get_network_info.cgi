#!/bin/sh

echo {

# Read network configuration
nexell_nettype=Static
nexell_netdevice=
nexell_macaddr=
nexell_ipaddress=
nexell_netmask=

if [ -s /etc/systemd/network/10-eth.network ] ; then
	nexell_nettype=`cat /etc/systemd/network/10-eth.network | grep DHCP`
	if [ -z nexell_nettype ]; then
		nexell_nettype="DHCP=false"
	fi
	nexell_nettype=${nexell_nettype/DHCP=/}
	if [ "${nexell_nettype}" == "yes" ]; then
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

if [ -s /config/hostname ] ; then	
	nexell_conf_hostname=`cat /config/hostname`
fi
if [ -s /etc/systemd/network/10-eth.network ] ; then	
	nexell_conf_ipaddress=`cat /etc/systemd/network/10-eth.network | grep Address | sed -e 's;^.*=;;' | sed -e 's;/.*$;;'`
	nexell_conf_netmask=`cat /etc/systemd/network/10-eth.network | grep Address | sed -e 's;^.*=;;' | sed -e 's;^.*/;;'`
	if [ "${nexell_conf_netmask}" == "24" ]; then
		nexell_conf_netmask=255.255.255.0
	elif [ "${nexell_conf_netmask}" == "26" ]; then
		nexell_conf_netmask=255.255.255.192
	elif [ "${nexell_conf_netmask}" == "28" ]; then
		nexell_conf_netmask=255.255.255.240
	elif [ "${nexell_conf_netmask}" == "30" ]; then
		nexell_conf_netmask=255.255.255.252
	else
		nexell_conf_netmask=
	fi
	nexell_conf_gateway=`cat /etc/systemd/network/10-eth.network | grep Gateway`
	nexell_conf_gateway=${nexell_conf_gateway/Gateway=/}
	nexell_conf_dnsservers=`cat /etc/systemd/network/10-eth.network | grep DNS`
	nexell_conf_dnsservers=${nexell_conf_dnsservers/DNS=/}
fi

echo \"conf_nettype\":\"${nexell_conf_nettype}\",
echo \"conf_hostname\":\"${nexell_conf_hostname}\",
echo \"conf_ipaddress\":\"${nexell_conf_ipaddress}\",
echo \"conf_netmask\":\"${nexell_conf_netmask}\",
echo \"conf_gateway\":\"${nexell_conf_gateway}\",
echo \"conf_dnsservers\":\"${nexell_conf_dnsservers}\"

echo }
