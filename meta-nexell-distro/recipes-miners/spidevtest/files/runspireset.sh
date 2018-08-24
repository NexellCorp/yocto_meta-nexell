#!/bin/sh

#RESET
echo ""
echo ""
echo "RESET"
echo ""
echo "spidevtest -OH -D /dev/spidev0.0 -v -p \"\x04\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x04\x00\x00\x00"

