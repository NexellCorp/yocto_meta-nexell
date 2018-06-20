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

if [ "${nexell_conf_nettype}" == "DHCP" ]; then
	echo "dhcp=true"								>  /config/network.conf
	echo "hostname=${nexell_conf_hostname}"			>> /config/network.conf
else
	echo "hostname=${nexell_conf_hostname}"			>  /config/network.conf
	echo "ipaddress=${nexell_conf_ipaddress}"			>> /config/network.conf
	echo "netmask=${nexell_conf_netmask}"				>> /config/network.conf
	echo "gateway=${nexell_conf_gateway}"				>> /config/network.conf
	echo "dnsservers=\"${nexell_conf_dnsservers}"\"	>> /config/network.conf
fi

/etc/init.d/network.sh
/etc/init.d/avahi restart > /dev/null

sleep 10s

echo "ok"
