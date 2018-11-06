#!/bin/sh

REBOOT_FLAG=0
CONFIG_MMC_FS=`/sbin/blkid | /bin/grep mmcblk0p5`
#LOG_MMC_FS=`/sbin/blkid | /bin/grep mmcblk0p6`
if [ ! "$CONFIG_MMC_FS" ]; then
	/sbin/mkfs.ext4 -j -F /dev/mmcblk0p5
	sync;
	mount /dev/mmcblk0p5 /config
	sync;
	cat /sys/class/net/eth0/address > /config/eth0addr
	sync;
	REBOOT_FLAG=1
fi

#if [ ! "$LOG_MMC_FS" ]; then
#	/sbin/mkfs.ext4 -j -F /dev/mmcblk0p6
#	sync;
#	REBOOT_FLAG=1
#fi

if [ $REBOOT_FLAG -ne 0 ]; then
#	echo "/dev/mmcblk0p5	/config			auto	rw,noexec	0 0" >> /etc/fstab
#	echo "/dev/mmcblk0p6	/var/mmclog		auto	rw,noexec	0 0" >> /etc/fstab
	sync;
	/sbin/reboot
fi

CONFIG_MMC_FS_MOUNT=`/bin/mount | /bin/grep mmcblk0p5`
if [ ! "$CONFIG_MMC_FS_MOUNT" ]; then
	mount /dev/mmcblk0p5 /config
	sync
fi

if [ ! -e /config/cgminer.conf ]; then
	cp /etc/cgminer.conf.factory /config/cgminer.conf
	sync
fi

if [ ! -e /config/eth0addr ]; then
	cat /sys/class/net/eth0/address > /config/eth0addr
	sync;
fi

if [ ! -e /etc/systemd/network/10-eth.network ]; then
	cp /etc/10-eth.network.factory /etc/systemd/network/10-eth.network
	sync
fi

