#!/bin/sh

cd /usr/lib

rm libMali_for_*
touch libMali_for_WAYLAND

ln -sf ./nexell/libMali-32-WAYLAND.so libMali.so
