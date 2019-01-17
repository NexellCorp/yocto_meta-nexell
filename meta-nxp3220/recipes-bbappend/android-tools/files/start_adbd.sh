#! /bin/sh
#
#  usb adb gadget function driver.
#

ID_VENDOR=0x18d1
ID_PRODUCT=0x0001	# for window driver
MANUFACT=`hostname`
PRODUCT=Gadget
SERIAL=`hostname`

CONFIG_DIR=/sys/kernel/config/usb_gadget/g1
CONFIG_FUNC=ffs.adb

echo "path         : $CONFIG_DIR"
echo "id           : $ID_VENDOR, $ID_PRODUCT"
echo "manufacturer : $MANUFACT"
echo "product      : $PRODUCT"
echo "serial       : $SERIAL"

if ! mount | grep /sys/kernel/config > /dev/null; then
        mount -t configfs none /sys/kernel/config
fi

mkdir -p $CONFIG_DIR
mkdir -p $CONFIG_DIR/strings/0x409
mkdir -p $CONFIG_DIR/configs/c.1
mkdir -p $CONFIG_DIR/configs/c.1/strings/0x409

echo $ID_VENDOR  > $CONFIG_DIR/idVendor
echo $ID_PRODUCT > $CONFIG_DIR/idProduct
echo $MANUFACT   > $CONFIG_DIR/strings/0x409/manufacturer
echo $PRODUCT    > $CONFIG_DIR/strings/0x409/product
echo $SERIAL     > $CONFIG_DIR/strings/0x409/serialnumber;

echo "function     : $CONFIG_FUNC"
mkdir -p $CONFIG_DIR/functions/$CONFIG_FUNC
if [ ! -e "$CONFIG_DIR/configs/c.1/$CONFIG_FUNC" ]; then
	ln -s $CONFIG_DIR/functions/$CONFIG_FUNC $CONFIG_DIR/configs/c.1;
fi

# Create Adb FunctionFS function
echo "Run ADB daemon"
mkdir -p /dev/usb-ffs/adb
mount -o uid=2000,gid=2000 -t functionfs adb /dev/usb-ffs/adb

adbd &
sleep 1

#
# Bring up USB (RESET)
# echo c0040000.dwc2otg > $CONFIG_DIR/UDC
#
echo "Bring up usb function '$CONFIG_DIR/UDC' ..."
ls /sys/class/udc/ > $CONFIG_DIR/UDC

