#! /bin/sh
#
#  This script support usb uac2 gadget function driver.
#  usage:
#	usb_uac2.sh <sample rate> <ch>
#

ID_VENDOR=0x18d1
ID_PRODUCT=0x0003

MANUFACT=`hostname`
PRODUCT=Gadget
SERIAL=`hostname`

USB_GADGET=/sys/kernel/config/usb_gadget
GADGET_NAME=g
GADGET_DIR=$USB_GADGET/$GADGET_NAME
GADGET_ADB=n

INSTANCE_NAME=uac2.usb0
UAC2_SAMPLELATE=16000
UAC2_CHANNEL=2
UAC2_CHMASK=0x3

if [  $1 ]; then
	case $1 in
	"16000" | "48000")
		UAC2_SAMPLELATE=$1;;
	*)
		echo "No support: sample rate $1 [16000,48000]"
		exit;;
	esac
fi

if [  $1 ]; then
	case $2 in
	"2")
		UAC2_CHANNEL=2
	 	UAC2_CHMASK=0x3;;
	"4")
		UAC2_CHANNEL=4
	 	UAC2_CHMASK=0xf;;
	"8")
		UAC2_CHANNEL=8
	 	UAC2_CHMASK=0xff;;
	*)
		echo "No support: channel $1 [2,4,8]"
		exit;;
	esac
fi

echo "======================================================="
echo "path         : $GADGET_DIR"
echo "gadget       : $GADGET_NAME"
echo "id           : $ID_VENDOR, $ID_PRODUCT"
echo "manufacturer : $MANUFACT"
echo "product      : $PRODUCT"
echo "serial       : $SERIAL"
echo "samplerate   : $UAC2_SAMPLELATE"
echo "channel      : $UAC2_CHANNEL $UAC2_CHMASK"
echo "function     : $INSTANCE_NAME"
echo "======================================================="

if ! mount | grep /sys/kernel/config > /dev/null; then
        mount -t configfs none /sys/kernel/config
fi

mkdir -p $GADGET_DIR
mkdir -p $GADGET_DIR/strings/0x409
mkdir -p $GADGET_DIR/configs/c.1
mkdir -p $GADGET_DIR/configs/c.1/strings/0x409

echo $ID_VENDOR  > $GADGET_DIR/idVendor
echo $ID_PRODUCT > $GADGET_DIR/idProduct
echo $MANUFACT   > $GADGET_DIR/strings/0x409/manufacturer
echo $PRODUCT    > $GADGET_DIR/strings/0x409/product
echo $SERIAL     > $GADGET_DIR/strings/0x409/serialnumber;

mkdir -p $GADGET_DIR/functions/$INSTANCE_NAME

echo $UAC2_SAMPLELATE > $GADGET_DIR/functions/$INSTANCE_NAME/p_srate
echo $UAC2_CHMASK > $GADGET_DIR/functions/$INSTANCE_NAME/p_chmask

if [ ! -e "$GADGET_DIR/configs/c.1/$INSTANCE_NAME" ]; then
	ln -s $GADGET_DIR/functions/$INSTANCE_NAME $GADGET_DIR/configs/c.1;
fi

echo "Bring up usb function '$GADGET_DIR/UDC' ..."

#
# Bring up USB (RESET)
# echo c0040000.dwc2otg > $GADGET_DIR/UDC
#
ls /sys/class/udc/ > $GADGET_DIR/UDC
