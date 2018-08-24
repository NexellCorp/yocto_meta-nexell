#!/bin/sh

echo "Demo TiaPlyaer"

export XDG_RUNTIME_DIR=/run/wayland
export LD_LIBRARY_PATH=$PWD

echo $LD_LIBRARY_PATH

export XDG_RUNTIME_DIR=/run/wayland
export `dbus-launch`

chmod +x ./TiaPlayer.elf
./TiaPlayer.elf -w1920 -h720 -c./ -r./




