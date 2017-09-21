#!/bin/sh

cd /usr/lib

rm libMali_for_*
touch libMali_for_FB

ln -sf ./nexell/libMali.so-32-FB libMali.so
