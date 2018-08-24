#!/bin/sh

echo "Autorun TiaPlayer"

ip link set can0 up type can bitrate 500000
ip link set can0 up type can bitrate 500000
echo "enable can0"

echo "start nx_3d_avm"
/nexell/avm_app/nx_3d_avm &

echo "start TiaPlayer"

export LD_LIBRARY_PATH=/nexell/cluster

echo $LD_LIBRARY_PATH

chmod +x /nexell/cluster/TiaPlayer.elf
/nexell/cluster/TiaPlayer.elf -w1920 -h720 -c/nexell/cluster -r/nexell/cluster &

