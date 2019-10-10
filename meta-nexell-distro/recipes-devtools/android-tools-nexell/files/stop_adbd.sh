#!/bin/sh
GADGET_DIR=/sys/kernel/config/usb_gadget/g1
PID_ADBD=`pidof adbd`

if [ -f "/run/adbd.pid" ]; then
    kill $PID_ADBD
fi

while [ -d "/dev/usb-ffs/adb" ]
do
    umount /dev/usb-ffs/adb
    rmdir /dev/usb-ffs/adb
done

if [ -d "${GADGET_DIR}/functions/ffs.adb" ]; then
    rm ${GADGET_DIR}/configs/c.1/ffs.adb
    rmdir ${GADGET_DIR}/configs/c.1/strings/0x409
    rmdir ${GADGET_DIR}/configs/c.1
    rmdir ${GADGET_DIR}/functions/ffs.adb
    rmdir ${GADGET_DIR}/strings/0x409
fi

