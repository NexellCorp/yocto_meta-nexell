#!/bin/sh

#WRITE_NONCE
echo ""
echo ""
echo "WRITE_NONCE"
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x16\x01\x66\xcb\x34\x00\x66\xcb\x34\x7f\x00\x00"
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x16\x02\x66\xcb\x34\x80\x66\xcb\x34\xff\x00\x00"
# RUN_JOB용으로는 이걸로 범위 지정

#SET_CONTROL
echo ""
echo ""
echo "SET_CONTROL"
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x12\x01\x00\x00\x00\x11\x00\x00"
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x12\x02\x00\x00\x00\x11\x00\x00"

#RUN_JOB
echo ""
echo ""
echo "RUN_JOB"
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x0B\x01\x00\x00\x00\x00\x00\x00"
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x0B\x02\x00\x00\x00\x00\x00\x00"
sleep 1

#READ_ID
echo ""
echo ""
echo "READ_ID"
echo ""
echo "spidevtest -OH -D /dev/spidev0.0 -v -p \"\x00\x01\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x00\x01\x00\x00\x00\x00\x00\x00"
echo ""
echo ""
echo "spidevtest -OH -D /dev/spidev0.0 -v -p \"\x00\x02\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x00\x02\x00\x00\x00\x00\x00\x00"

#READ_JOB_ID
echo ""
echo ""
echo "READ_JOB_ID"
echo ""
echo " spidevtest -OH -D /dev/spidev0.0 -v -p \"\x0C\x00\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x0C\x00\x00\x00\x00\x00\x00\x00"
spidevtest -OH -D /dev/spidev0.0 -v -p "\x0C\x00\x00\x00\x00\x00\x00\x00"

#READ_RESULT 2
echo ""
echo ""
echo "READ_RESULT 2"
echo ""
echo " spidevtest -OH -D /dev/spidev0.0 -v -p \"\x0D\x02\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x0D\x02\x00\x00\x00\x00\x00\x00"

#READ_JOB_ID
echo ""
echo ""
echo "READ_JOB_ID"
echo ""
echo " spidevtest -OH -D /dev/spidev0.0 -v -p \"\x0C\x00\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x0C\x00\x00\x00\x00\x00\x00\x00"

#READ_HASH
echo ""
echo ""
echo "READ_HASH"
echo ""
echo " spidevtest -OH -D /dev/spidev0.0 -v -p \"\x20\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x20\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
echo ""
echo ""
echo " spidevtest -OH -D /dev/spidev0.0 -v -p \"\x20\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x20\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"

#READ_RESULT 1
echo ""
echo ""
echo "READ_RESULT 1"
echo ""
echo " spidevtest -OH -D /dev/spidev0.0 -v -p \"\x0D\x01\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x0D\x01\x00\x00\x00\x00\x00\x00"

#READ_JOB_ID
echo ""
echo ""
echo "READ_JOB_ID"
echo ""
echo " spidevtest -OH -D /dev/spidev0.0 -v -p \"\x0C\x01\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x0C\x01\x00\x00\x00\x00\x00\x00"
echo ""
echo ""
echo " spidevtest -OH -D /dev/spidev0.0 -v -p \"\x0C\x02\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x0C\x02\x00\x00\x00\x00\x00\x00"

#READ_RESULT 1
echo ""
echo ""
echo "READ_RESULT 1"
echo ""
echo " spidevtest -OH -D /dev/spidev0.0 -v -p \"\x0D\x01\x00\x00\x00\x00\x00\x00\""
echo ""
spidevtest -OH -D /dev/spidev0.0 -v -p "\x0D\x01\x00\x00\x00\x00\x00\x00"

