#!/bin/sh

echo "Demo TiaPlyaer"

export LD_LIBRARY_PATH=$PWD

chmod +x ./TiaPlayer.elf
./TiaPlayer.elf -w1920 -h720 -c./ -r./

