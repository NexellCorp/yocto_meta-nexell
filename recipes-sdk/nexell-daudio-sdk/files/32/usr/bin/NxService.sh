#!/bin/sh

# Add services to run below
/usr/bin/NxDAudioManager &
sleep 6
/usr/bin/NxBTServiceR &
/nexell/daudio/NxBTPhoneR/NxBTPhoneR -platform wayland &
