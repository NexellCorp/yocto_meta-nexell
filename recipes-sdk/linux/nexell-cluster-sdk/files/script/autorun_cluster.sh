#!/bin/sh

echo "Autorun script"

ip link set can0 up type can bitrate 500000
ip link set can0 up type can bitrate 500000
echo "enable can0"

echo "start nx_3d_avm"
chmod +x /nexell/avm_app/nx_3d_avm
/nexell/avm_app/nx_3d_avm &

chmod +x /nexell/fakenavi/video_api_test

sh /nexell/cluster/run.sh &
