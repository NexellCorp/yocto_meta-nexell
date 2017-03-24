#!/bin/sh

mount -t configfs none /sys/kernel/config

GADGET_DIR=/sys/kernel/config/usb_gadget/g1
SERIAL_PATH=/proc/device-tree/serial-number

#Mount ConfigFS and create Gadget
if ! [ -d "${GADGET_DIR}" ]; then
	mkdir ${GADGET_DIR}
fi

#Set default Vendor and Product IDs for now
echo 0x18D1 > ${GADGET_DIR}/idVendor
echo 0x0001 > ${GADGET_DIR}/idProduct

#Create English strings and add random deviceID
if ! [ -d "${GADGET_DIR}/strings/0x409" ]; then
	mkdir ${GADGET_DIR}/strings/0x409
fi

#Set serial number
if [ -f ${SERIAL_PATH} ]; then
	cat ${SERIAL_PATH} > ${GADGET_DIR}/strings/0x409/serialnumber
else
	cat /etc/hostname > ${GADGET_DIR}/strings/0x409/serialnumber
fi
echo NEXELL > ${GADGET_DIR}/strings/0x409/manufacturer
cat /etc/hostname > ${GADGET_DIR}/strings/0x409/product

#Create gadget configuration
if ! [ -d "${GADGET_DIR}/configs/c.1" ]; then
	mkdir ${GADGET_DIR}/configs/c.1
	mkdir ${GADGET_DIR}/configs/c.1/strings/0x409
fi
echo 120 > ${GADGET_DIR}/configs/c.1/MaxPower
echo "Conf 1" > ${GADGET_DIR}/configs/c.1/strings/0x409/configuration


#Create Adb FunctionFS function
#And link it to the gadget configuration
if ! [ -d "${GADGET_DIR}/functions/ffs.adb" ]; then
	mkdir ${GADGET_DIR}/functions/ffs.adb
	ln -s ${GADGET_DIR}/functions/ffs.adb ${GADGET_DIR}/configs/c.1/ffs.adb
fi

if ! [ -d "/dev/usb-ffs/adb" ]; then
	mkdir -p /dev/usb-ffs/adb
	mount -o uid=2000,gid=2000 -t functionfs adb /dev/usb-ffs/adb
fi

#Start adbd application
/usr/bin/adbd&
echo $! > /run/adbd.pid
sleep 2

if [ "`cat ${GADGET_DIR}/UDC`" == "c0040000.dwc2otg" ]; then
	echo "" > ${GADGET_DIR}/UDC
fi
echo c0040000.dwc2otg > ${GADGET_DIR}/UDC
