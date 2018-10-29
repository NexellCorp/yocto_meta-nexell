#!/bin/sh

echo "Demo TiaPlyaer"

export LD_LIBRARY_PATH=/nexell/convergence-svmc

echo $LD_LIBRARY_PATH

chmod +x /nexell/convergence-svmc/TiaPlayer.elf
/nexell/convergence-svmc/TiaPlayer.elf -w1920 -h720 -c/nexell/convergence-svmc -r/nexell/convergence-svmc




