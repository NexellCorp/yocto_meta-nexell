#!/bin/sh

REBOOT_FLAG=0
CONFIG_MMC_FS=`/sbin/blkid | /bin/grep mmcblk0p5`
#LOG_MMC_FS=`/sbin/blkid | /bin/grep mmcblk0p6`
if [ ! "$CONFIG_MMC_FS" ]; then
	/sbin/mkfs.ext4 -j -F /dev/mmcblk0p5
	sync;
	mount /dev/mmcblk0p5 /config
	sync;
	echo "config is not formated" > /dev/ttySAC3
	REBOOT_FLAG=1
fi

#if [ ! "$LOG_MMC_FS" ]; then
#	/sbin/mkfs.ext4 -j -F /dev/mmcblk0p6
#	sync;
#	REBOOT_FLAG=1
#fi

HOSTNAME_PREFIX=`/bin/hostname | awk -F- '{print $1$2}'`
if [ "$HOSTNAME_PREFIX" != "poc2ctrlbd" ]; then
	ENVMAC=`/sbin/fw_printenv | grep ethaddr=a4 | awk -F= '{print $2}'| tr -d : | tr 'a-z' 'A-Z'`
	echo "poc2-ctrlbd-$ENVMAC" > /etc/hostname
	echo "hostname is not set" > /dev/ttySAC3
	REBOOT_FLAG=1
fi

if [ $REBOOT_FLAG -ne 0 ]; then
#	echo "/dev/mmcblk0p5	/config			auto	rw,noexec	0 0" >> /etc/fstab
#	echo "/dev/mmcblk0p6	/var/mmclog		auto	rw,noexec	0 0" >> /etc/fstab
	sync;
	/sbin/reboot -f
fi

CONFIG_MMC_FS_MOUNT=`/bin/mount | /bin/grep mmcblk0p5`
if [ ! "$CONFIG_MMC_FS_MOUNT" ]; then
	mount /dev/mmcblk0p5 /config
	sync
fi

#MACADDRUSER_FLAG=`cat /config/cgminer.conf | grep macxxxxxx`
#if [ "$MACADDRUSER_FLAG" ]; then
#	cp /etc/cgminer.conf.factory /config/cgminer.conf
#fi

#if [ ! -e /etc/systemd/network/10-eth.network ]; then
#	cp /etc/10-eth.network.factory /etc/systemd/network/10-eth.network
#	sync
#fi

TEST_MODE=`cat /sys/class/gpio/gpio15/value`
if [ "$TEST_MODE" = "0" ]; then
	echo "test mode"
	exit
fi

if [ ! -e /etc/systemd/network/10-eth.network ]; then
	/usr/sbin/avahi-autoipd -wD eth0
else
	/sbin/ifup eth0
fi

TIMEOUT_MAC=0
while [ $(cat /sys/class/net/eth0/address | awk -F: '{print $1 $2 $3}') != "a45f9b" ]
do
	sleep 1
	sync
	TIMEOUT_MAC=$(($TIMEOUT_MAC+1))
	if [ $TIMEOUT_MAC -lt 100 ]; then
		/usr/bin/logger -p local0.error "MAC address is not normal"
		cat /sys/class/net/eth0/address | /usr/bin/logger -p local0.error
	fi
done

if [ ! -e /config/eth0addr ]; then
	cat /sys/class/net/eth0/address > /config/eth0addr
fi

MYMAC=`cat /sys/class/net/eth0/address | tr -d : | tr 'a-z' 'A-Z'`

DEFAULT_MACADDRUSER_FLAG=`cat /etc/cgminer.conf.factory | grep macxxxxxx`
if [ "$DEFAULT_MACADDRUSER_FLAG" ]; then
	sed -i -e 's/macxxxxxx/'"$MYMAC"'/g' /etc/cgminer.conf.factory
fi

create_default_conf_file()
{
(
cat <<'EOF'
{
"pools" : [
{
"url" : "stratum+tcp://hansandopool.com:3333",
"user" : "12ships_DaeJeon.macxxxxxx",
"pass" : "x"
},
{
"url" : "stratum+tcp://hansandopool.com:3333",
"user" : "12ships_DaeJeon.macxxxxxx",
"pass" : "x"
}
]
,
"api-listen" : true,
"api-network" : true,
"api-allow" : "W:0/0",
"temp-cutoff" : 100,
"tsb1101-volt": "400"
}

EOF
) > /etc/cgminer.conf.factory
}

if [ ! -e /config/cgminer.conf ]; then
	if [ ! -e /etc/cgminer.conf.factory ]; then
		create_default_conf_file
		MACAD=$(cat /sys/class/net/eth0/address | tr -d : | tr 'a-z' 'A-Z')
		sed -i -e 's/macxxxxxx/'"$MACAD"'/g' /etc/cgminer.conf.factory
	fi
	cp /etc/cgminer.conf.factory /config/cgminer.conf
fi

