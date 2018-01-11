#!/bin/sh

#source /usr/bin/nexell-qt5-touchsetup.sh
cd /usr/share/qtsmarthome-1.0/
export DISPLAY=:0.0
./smarthome --platform xcb
