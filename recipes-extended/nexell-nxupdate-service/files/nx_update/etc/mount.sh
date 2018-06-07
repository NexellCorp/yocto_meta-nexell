#!/bin/bash

count=0

while read StrBLOCK StrUUID StrFSTYPE
do
	#echo "${StrBLOCK} ${StrUUID} ${StrFSTYPE}"
	BLOCK=$(echo ${StrBLOCK} | cut -d':' -f1)
	FSTYPE=$(echo ${StrFSTYPE} | cut -d'"' -f4)
	#echo "${BLOCK}"
	#echo "${FSTYPE}"
	#echo "${count}"
	MOUNT_DST="/mnt/${count}"
	#echo "${MOUNT_DST}"
	mkdir -p $MOUNT_DST
	if [ ${FSTYPE} == "ntfs" ]; then
		mount.ntfs-3g  $BLOCK $MOUNT_DST
	else
		if [ ${FSTYPE} == "exfat" ]; then
			mount.exfat $BLOCK $MOUNT_DST
		else
			mount -t $FSTYPE $BLOCK $MOUNT_DST
		fi
	fi
	count=`expr $count + 1`
done < $1
