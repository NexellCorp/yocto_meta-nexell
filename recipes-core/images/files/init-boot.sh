#!/bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

mkdir /proc
mkdir /sys
mount -t proc proc /proc
mount -t sysfs sysfs /sys

ln -sf bin/busybox linuxrc
rm -rf run
ln -sf tmp run

mount -t devtmpfs none /dev
mount -t proc none /proc
mount -t sysfs none /sys

exec sh
