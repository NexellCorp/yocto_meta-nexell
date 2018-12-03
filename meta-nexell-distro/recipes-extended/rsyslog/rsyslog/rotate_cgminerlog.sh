#!/bin/sh
if [ -e /config/log/cgminer/log.6 ]; then
	mv -f /config/log/cgminer/log.6 /config/log/cgminer/log.7
fi
if [ -e /config/log/cgminer/log.5 ]; then
	mv -f /config/log/cgminer/log.5 /config/log/cgminer/log.6
fi
if [ -e /config/log/cgminer/log.4 ]; then
	mv -f /config/log/cgminer/log.4 /config/log/cgminer/log.5
fi
if [ -e /config/log/cgminer/log.3 ]; then
	mv -f /config/log/cgminer/log.3 /config/log/cgminer/log.4
fi
if [ -e /config/log/cgminer/log.2 ]; then
	mv -f /config/log/cgminer/log.2 /config/log/cgminer/log.3
fi
if [ -e /config/log/cgminer/log.1 ]; then
	mv -f /config/log/cgminer/log.1 /config/log/cgminer/log.2
fi
if [ -e /config/log/cgminer/log ]; then
	mv -f /config/log/cgminer/log /config/log/cgminer/log.1
fi
