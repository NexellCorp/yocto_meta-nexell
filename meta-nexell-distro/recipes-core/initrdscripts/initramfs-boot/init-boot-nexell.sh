#!/bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

mkdir /proc
mkdir /sys
mount -t proc proc /proc
mount -t sysfs sysfs /sys
mount -t devtmpfs devtmpfs /dev

hostname nexell

exec /bin/bash
