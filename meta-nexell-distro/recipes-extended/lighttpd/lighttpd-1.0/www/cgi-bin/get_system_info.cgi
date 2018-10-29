#!/bin/sh

echo {

# Read network configuration
ant_minertype="Nexell Bitminer TSB1101"
ant_nettype=
ant_netdevice=
ant_macaddr=
ant_hostname=
ant_ipaddress=
ant_netmask=
ant_gateway=
ant_dnsservers=

ant_system_filesystem_version=

if [ -s /etc/systemd/network/10-eth.network ] ; then
	ant_nettype=`cat /etc/systemd/network/10-eth.network | grep DHCP`
	if [ -z ant_nettype ]; then
		ant_nettype="DHCP=false"
	fi
	ant_nettype=${ant_nettype/DHCP=/}
	if [ "${ant_nettype}" == "yes" ]; then
		ant_nettype=DHCP
	else
		ant_nettype=Static
	fi
	
	ant_host_name=`/bin/hostname`
#	ant_host_name=`cat /etc/systemd/network/10-eth.network | grep hostname`
#	ant_host_name=${ant_host_name/hostname=/}
	ant_ipaddress=`cat /etc/systemd/network/10-eth.network | grep Address | sed -e 's;^.*=;;' | sed -e 's;/.*$;;'`
	ant_netmask=`cat /etc/systemd/network/10-eth.network | grep Address | sed -e 's;^.*=;;' | sed -e 's;^.*/;;'`
	if [ "${ant_netmask}" == "24" ]; then
		ant_netmask=255.255.255.0
	elif [ "${ant_netmask}" == "26" ]; then
		ant_netmask=255.255.255.192
	elif [ "${ant_netmask}" == "28" ]; then
		ant_netmask=255.255.255.240
	elif [ "${ant_netmask}" == "30" ]; then
		ant_netmask=255.255.255.252
	else
		ant_netmask=
	fi
	ant_gateway=`cat /etc/systemd/network/10-eth.network | grep Gateway`
	ant_gateway=${ant_gateway/Gateway=/}
	ant_dnsservers=`cat /etc/systemd/network/10-eth.network | grep DNS`
	ant_dnsservers=${ant_dnsservers/DNS=/}
else
	ant_host_name=`/bin/hostname`
	ant_ipaddress=`ip addr | grep "inet " | grep brd | awk '{print $2}' | awk -F/ '{print $1}'`
fi

ant_tmp=`ifconfig | grep eth`
i=0
for ant_var in ${ant_tmp}
do
	case ${i} in
		0 )
		ant_netdevice=${ant_var}
		;;
		4 )
		ant_macaddr=${ant_var}
		;;
	esac
	i=`expr $i + 1`
done

ant_tmp=`ifconfig | grep "inet addr"`
i=0
for ant_var in ${ant_tmp}
do
	case ${i} in
		1 )
		ant_ipaddress=${ant_var}
		ant_ipaddress=${ant_ipaddress/addr:/}
		;;
		3 )
		ant_netmask=${ant_var}
		ant_netmask=${ant_netmask/Mask:/}
		;;
	esac
	i=`expr $i + 1`
done

echo \"minertype\":\"${ant_minertype}\",
echo \"nettype\":\"${ant_nettype}\",
echo \"netdevice\":\"${ant_netdevice}\",
echo \"macaddr\":\"${ant_macaddr}\",
echo \"hostname\":\"${ant_host_name}\",
echo \"ipaddress\":\"${ant_ipaddress}\",
echo \"netmask\":\"${ant_netmask}\",
echo \"gateway\":\"${ant_gateway}\",
echo \"dnsservers\":\"${ant_dnsservers}\",

#Read system uptime status
ant_curtime=
ant_uptime=
ant_loadaverage=

ant_tmp=`uptime`
ant_tmp=${ant_tmp/,/ }
i=0
for ant_var in ${ant_tmp}
do
	case ${i} in
		0 )
		ant_curtime=${ant_var}
		;;
		2 )
		ant_uptime=${ant_var}
		;;
	esac
	i=`expr $i + 1`
done

ant_loadaverage=${ant_tmp#*average:}
ant_loadaverage=${ant_loadaverage# }
ant_loadaverage=${ant_loadaverage% }

echo \"curtime\":\"${ant_curtime}\",
echo \"uptime\":\"${ant_uptime}\",
echo \"loadaverage\":\"${ant_loadaverage}\",

#Read system memory
ant_mem_total=
ant_mem_used=
ant_mem_free=
ant_mem_buffers=
ant_mem_cached=

ant_tmp=`free | grep Mem:`
ant_tmp=${ant_tmp/Mem:/}
i=0
for ant_var in ${ant_tmp}
do
	case ${i} in
		0 )
		ant_mem_total=${ant_var}
		;;
		1 )
		ant_mem_used=${ant_var}
		;;
		2 )
		ant_mem_free=${ant_var}
		;;
		3)
		ant_mem_cached=${ant_var}
		;;
		4 )
		ant_mem_buffers=${ant_var}
		;;
	esac
	i=`expr $i + 1`
done;

echo \"mem_total\":\"${ant_mem_total}\",
echo \"mem_used\":\"${ant_mem_used}\",
echo \"mem_free\":\"${ant_mem_free}\",
echo \"mem_buffers\":\"${ant_mem_buffers}\",
echo \"mem_cached\":\"${ant_mem_cached}\",

ant_cgminer_version=`cgminer-api -o version`
if [ "${ant_cgminer_version}" == "Socket connect failed: Connection refused" ]; then
	ant_cgminer_version=
else
	ant_cgminer_version=${ant_cgminer_version#*CGMiner=}
	ant_cgminer_version=${ant_cgminer_version%%,API=*}
fi

ant_hwv1=x
ant_hwv2=x
ant_hwv3=x
ant_hwv4=x
ant_hwv=`cgminer-api -o stats`
if [ "${ant_hwv}" != "Socket connect failed: Connection refused" ]; then
	ant_hwv1=${ant_hwv#*hwv1=}
	ant_hwv1=${ant_hwv1%%,hwv2=*}
	
	ant_hwv2=${ant_hwv#*hwv2=}
	ant_hwv2=${ant_hwv2%%,hwv3=*}
	
	ant_hwv3=${ant_hwv#*hwv3=}
	ant_hwv3=${ant_hwv3%%,hwv4=*}
	
	ant_hwv4=${ant_hwv#*hwv4=}
	ant_hwv4=${ant_hwv4%%,fan_num=*}
fi

ant_system_mode=`uname -o`
ant_system_kernel_version=`uname -srv`
#ant_system_filesystem_version=`cat /usr/bin/compile_time`
ant_minertype=`sed -n 2p /usr/bin/compile_time`
ant_system_filesystem_version=`sed -n 1p /usr/bin/compile_time`

echo \"system_mode\":\"${ant_system_mode}\",
echo \"ant_hwv1\":\"${ant_hwv1}\",
echo \"ant_hwv2\":\"${ant_hwv2}\",
echo \"ant_hwv3\":\"${ant_hwv3}\",
echo \"ant_hwv4\":\"${ant_hwv4}\",
echo \"system_kernel_version\":\"${ant_system_kernel_version}\",
echo \"system_filesystem_version\":\"${ant_system_filesystem_version}\", 
echo \"cgminer_version\":\"${ant_cgminer_version}\"

echo }
