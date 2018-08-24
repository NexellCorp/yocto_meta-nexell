#!/bin/sh

PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin
DAEMON=/usr/bin/cgminer
NAME=cgminer
DESC="Cgminer daemon"
CONFIG_NAME="/config/asic-freq.config"
USER_SETTING="/config/user_setting"
PARAMS=""
set -e
#set -x
test -x "$DAEMON" || exit 0

do_start() {
	echo 36 > /sys/class/gpio/export
	echo out > /sys/class/gpio/gpio36/direction
	cat /sys/class/gpio/gpio36/value
	echo 0 > /sys/class/gpio/gpio36/value
	if [ ! -e /config/cgminer.conf ]; then
		cp /config/cgminer.conf.factory /config/cgminer.conf
	fi
	echo PARAMS = $PARAMS
	echo 1 > /sys/class/gpio/gpio36/value
#	start-stop-daemon -b -S -x screen -- -S cgminer -t cgminer -m -d "$DAEMON" $PARAMS --api-listen --default-config /config/cgminer.conf
}

do_stop() {
        killall -9 cgminer || true
}
case "$1" in
  start)
        echo -n "Starting $DESC: "
	do_start
        echo "$NAME."
        ;;
  stop)
        echo -n "Stopping $DESC: "
	do_stop
        echo "$NAME."
        ;;
  restart|force-reload)
        echo -n "Restarting $DESC: "
        do_stop
        do_start
        echo "$NAME."
        ;;
  *)
        N=/etc/init.d/$NAME
        echo "Usage: $N {start|stop|restart|force-reload}" >&2
        exit 1
        ;;
esac

exit 0
