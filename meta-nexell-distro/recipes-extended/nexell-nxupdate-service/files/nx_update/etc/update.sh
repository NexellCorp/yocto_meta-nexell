#!/bin/bash

function update_files_check()
{
	echo "update_files_check"
	echo "${1}"
	FILE=`cat ${1}/partmap_update.txt`
	for var in $FILE;
	do
		filename=`echo $var | cut -d';' -f1 | cut -d':' -f5`
#		echo "filename = ${filename}"
		if [ -n "${filename##+([[:space:]])}" ]; then
			if [ -f "${1}${filename}" ]; then
				echo "${1}${filename} file exist"
			else
				echo "${1}/${filename} not exist"
				/usr/bin/psplash-write "MSG Update file not found"
			fi
			sleep 1
		fi
	done
	echo "File check is finished"
	update_process ${1}
}

function update_check()
{
	if [ -f "update_cmd.txt" ]; then
		echo "update_cmd exist"
		update_files_check $1
	else
		echo "update_cmd does not exist"
		/usr/bin/psplash-write "MSG Update command file not found"
	fi
	sleep 1
}

function update_process()
{
	/usr/bin/psplash-write "MSG UPDATE PROCESS at ${1}"
	sleep 3
	/usr/bin/nx_update ${1}
	while [ 1 ]
	do
		echo -n "."
		sleep 1
	done
}

## Start at here##
echo "Enter the nexell update"
/usr/bin/psplash -n &
/usr/bin/psplash-write "MSG Update ready..."
sleep 3
/usr/bin/psplash-write "MSG Check mount block devices"
sleep 6
blkid > /tmp/blk_list.txt
/etc/mount.sh /tmp/blk_list.txt

echo "Mount done"
DIRS=`ls -d /mnt/*/`

for i in ${DIRS};do
	/usr/bin/psplash-write "MSG Entering directory : $i";
	cd $i;
	update_check $i;
	cd ../;
done

sleep 3
/usr/bin/psplash-write "MSG Update failed, so reboot..."
sleep 3
reboot
while [ 1 ]
	do
		echo -n "."
		sleep 1
done
