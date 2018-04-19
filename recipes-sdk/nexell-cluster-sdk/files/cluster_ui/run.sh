#!/bin/sh

echo "Demo TiaPlyaer"

export LD_LIBRARY_PATH=/nexell/cluster/
export XDG_RUNTIME_DIR=/run/user/0

chmod +x /nexell/cluster/TiaPlayer.elf
/nexell/cluster/TiaPlayer.elf -w1920 -h720 -c/nexell/cluster/ -r/nexell/cluster/

