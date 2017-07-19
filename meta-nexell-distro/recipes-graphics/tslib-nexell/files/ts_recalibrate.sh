#!/bin/sh

CAL_VALUES1=`ts_calibrate|tail -n 1`
#echo $CAL_VALUES1
CAL_VALUES2=`echo $CAL_VALUES1|cut -c 24-`
echo "'$CAL_VALUES2'" > /etc/pointercal
