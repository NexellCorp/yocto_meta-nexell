#!/bin/bash

function update_files_check()
{
#	local DIRS=`ls -1 ${1}`
	echo "update_files_check"
	echo "${1}"
#	echo "${DIRS}"
	FILE=`cat ${1}/partmap_update.txt`
	for var in $FILE;
	do
		filename=`echo $var | cut -d';' -f1 | cut -d':' -f5`
		#echo "filename = ${filename} "
		if [ -n "${filename##+([[:space:]])}" ]; then
			if [ -f "${1}${filename}" ]; then
				echo "${1}${filename} File exist"
			else
				echo "${1}/${filename} not exist"
				/usr/bin/psplash-write "MSG update file not found Reboot..."
				reboot
			fi
		#else
		#	echo "file null"
		fi
	done
	echo "File Check Finished"
	update_process ${1}
}


function update_check()
{
	if [ -f "update_cmd.txt" ]; then
		echo "update_cmd exist"
		update_files_check $1
	else
		echo "update_cmd not exist"
		#/usr/bin/psplash-write "MSG update file not found"
	fi
}

function update_process()
{
	echo "UPDATE PROCESS at ${1}"
	/usr/bin/nx_update ${1}
	reboot
	while [ 1 ]
	do
		echo -n "."
		sleep 1
	done
}
## Start at here##

echo "Enter the Nexell Update"
blkid > /tmp/blk_list.txt
/etc/mount.sh /tmp/blk_list.txt
/usr/bin/psplash -n &


echo "Mount done"
DIRS=`ls -d /mnt/*/`

for i in ${DIRS};do
	echo "Entering directory=$i";
	cd $i;
	update_check $i;
	cd ../;
done


echo "Finished the Nexell Update"
sleep 3
/usr/bin/psplash-write "MSG update file not found Reboot..."
sleep 3
reboot
while [ 1 ]
	do
		echo -n "."
		sleep 1
done
