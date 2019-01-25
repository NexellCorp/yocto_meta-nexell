#!/bin/sh

echo "<service-group>
  <name replace-wildcards=\"yes\">%h</name>
  <service>
    <type>_autoip._tcp</type>
  </service>
</service-group>
" > /etc/avahi/services/autoip.service

systemctl restart avahi-daemon

while [ ! -e "/etc/systemd/network/10-eth.network" ]; do
    sleep 1
    IPMASTER_IP=`avahi-browse _bitmineripmaster._tcp -rt -p | grep "=;eth0;IPv4" | awk -F';' '{print $8}'`
	MYMAC=`cat /sys/class/net/eth0/address | tr -d : | tr 'a-z' 'A-Z'`
    wget http://"$IPMASTER"/mac_"$MYMAC" /etc/systemd/network/10-eth.network
done
sync
rm -rf /etc/avahi/services/autoip.service
reboot -f
