#!/bin/sh
CONFIG_DIR=/sys/kernel/config/usb_gadget/g1
CONFIG_FUNC=ffs.adb
TASK_PID=`pidof adbd`

kill $TASK_PID

# must be unmount adb before
# execute 'rmdir ${CONFIG_DIR}/functions/$CONFIG_FUNC'
if [ -d "/dev/usb-ffs/adb" ]; then
    umount /dev/usb-ffs/adb
    rmdir /dev/usb-ffs/adb
fi

if [ -d "${CONFIG_DIR}/functions/$CONFIG_FUNC" ]; then
    rm ${CONFIG_DIR}/configs/c.1/$CONFIG_FUNC
    rmdir ${CONFIG_DIR}/configs/c.1/strings/0x409
    rmdir ${CONFIG_DIR}/configs/c.1
    rmdir ${CONFIG_DIR}/functions/$CONFIG_FUNC
    rmdir ${CONFIG_DIR}/strings/0x409
fi
