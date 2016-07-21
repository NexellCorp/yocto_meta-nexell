#!/bin/sh

rm /etc/udev/rules.d/ws-calibrate.rules

CAL_VALUES=`weston-calibrator|cut -c21-`
echo 'SUBSYSTEM=="input", ENV{WL_CALIBRATION}="'$CAL_VALUES'"' > $WS_CALUDEV_FILE
