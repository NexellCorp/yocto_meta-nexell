#!/bin/sh

REBOOT_FLAG=0
CONFIG_MMC_FS=`/sbin/blkid | /bin/grep mmcblk0p5`
#LOG_MMC_FS=`/sbin/blkid | /bin/grep mmcblk0p6`
if [ ! "$CONFIG_MMC_FS" ]; then
	/sbin/mkfs.ext4 -j -F /dev/mmcblk0p5
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

if [ ! -e /config/network.conf ]; then
	cp /etc/network.conf.factory /config/network.conf
	sync
fi

