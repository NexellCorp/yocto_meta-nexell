#!/bin/sh
#set -x

nexell_pool1url=
nexell_pool1user=
nexell_pool1pw=
nexell_pool2url=
nexell_pool2user=
nexell_pool2pw=
nexell_pool3url=
nexell_pool3user=
nexell_pool3pw=
nexell_nobeeper=
nexell_notempoverctrl=
nexell_freq=
nexell_voltage=

nexell_input=`cat /dev/stdin`
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
		nexell_pool1url=${nexell_var/_nexell_pool1url=/}
		;;
		1 )
		nexell_pool1user=${nexell_var/_nexell_pool1user=/}
		;;
		2 )
		nexell_pool1pw=${nexell_var/_nexell_pool1pw=/}
		;;
		3 )
		nexell_pool2url=${nexell_var/_nexell_pool2url=/}
		;;
		4 )
		nexell_pool2user=${nexell_var/_nexell_pool2user=/}
		;;
		5 )
		nexell_pool2pw=${nexell_var/_nexell_pool2pw=/}
		;;
		6 )
		nexell_pool3url=${nexell_var/_nexell_pool3url=/}
		;;
		7 )
		nexell_pool3user=${nexell_var/_nexell_pool3user=/}
		;;
		8 )
		nexell_pool3pw=${nexell_var/_nexell_pool3pw=/}
		;;
		9 )
		nexell_nobeeper=${nexell_var/_nexell_nobeeper=/}
		;;
		10 )
		nexell_notempoverctrl=${nexell_var/_nexell_notempoverctrl=/}
		;;
		11 )
		nexell_freq=${nexell_var/_nexell_freq=/}
		;;
		12 )
		nexell_voltage=${nexell_var/_nexell_voltage=/}
		;;
	esac
	i=`expr $i + 1`
done

echo "{"													>  /config/cgminer.conf
echo "\"pools\" : ["										>> /config/cgminer.conf
echo "{"													>> /config/cgminer.conf
echo "\"url\" : \"${nexell_pool1url}\","						>> /config/cgminer.conf
echo "\"user\" : \"${nexell_pool1user}\","						>> /config/cgminer.conf
echo "\"pass\" : \"${nexell_pool1pw}\""						>> /config/cgminer.conf
echo "},"													>> /config/cgminer.conf
echo "{"													>> /config/cgminer.conf
echo "\"url\" : \"${nexell_pool2url}\","						>> /config/cgminer.conf
echo "\"user\" : \"${nexell_pool2user}\","						>> /config/cgminer.conf
echo "\"pass\" : \"${nexell_pool2pw}\""						>> /config/cgminer.conf
echo "},"													>> /config/cgminer.conf
echo "{"													>> /config/cgminer.conf
echo "\"url\" : \"${nexell_pool3url}\","						>> /config/cgminer.conf
echo "\"user\" : \"${nexell_pool3user}\","						>> /config/cgminer.conf
echo "\"pass\" : \"${nexell_pool3pw}\""						>> /config/cgminer.conf
echo "}"													>> /config/cgminer.conf
echo "]"													>> /config/cgminer.conf
echo ","													>> /config/cgminer.conf
echo "\"api-listen\" : true,"								>> /config/cgminer.conf
echo "\"api-network\" : true,"								>> /config/cgminer.conf
echo "\"api-allow\" : \"W:0/0\","							>> /config/cgminer.conf
if [ "${nexell_nobeeper}" = "true" ]; then
	echo "\"bitmain-nobeeper\" : true,"						>> /config/cgminer.conf
fi
if [ "${nexell_notempoverctrl}" = "true" ]; then
	echo "\"bitmain-notempoverctrl\" : true,"				>> /config/cgminer.conf
fi
echo "\"bitmain-freq\" : \"${nexell_freq}\","					>> /config/cgminer.conf
echo "\"bitmain-voltage\" : \"${nexell_voltage}\""				>> /config/cgminer.conf
echo "}"													>> /config/cgminer.conf

sleep 1s

/bin/systemctl restart cgminer.service
#/etc/init.d/cgminer.sh restart >/dev/null 2>&1

sleep 5s

echo "ok"
