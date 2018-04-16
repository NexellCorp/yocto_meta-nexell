#!/bin/sh

echo "Demo TiaPlyaer"

export LD_LIBRARY_PATH=$PWD
export XDG_RUNTIME_DIR=/run/user/0

chmod +x ./TiaPlayer.elf
./TiaPlayer.elf -w1920 -h720 -c./ -r./

