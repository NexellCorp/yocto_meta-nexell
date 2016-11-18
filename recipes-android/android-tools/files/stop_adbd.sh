#!/bin/sh

PID_ADBD=`pidof adbd`
kill $PID_ADBD

rm /sys/kernel/config/usb_gadget/g1/configs/c.1/ffs.adb
rmdir /sys/kernel/config/usb_gadget/g1/configs/c.1/strings/0x409
rmdir /sys/kernel/config/usb_gadget/g1/configs/c.1
rmdir /sys/kernel/config/usb_gadget/g1/functions/ffs.adb
rmdir /sys/kernel/config/usb_gadget/g1/strings/0x409
