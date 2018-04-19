#!/bin/sh
#
### BEGIN INIT INFO
# Provides: cluster
# Required-Start: $local_fs $remote_fs
# Required-Stop: $local_fs $remote_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
### END INIT INFO

echo "========== /etc/init.d/cluster "

killproc() {
        pid=`/bin/pidof $1`
        [ "$pid" != "" ] && kill $pid
}

read CMDLINE < /proc/cmdline
for x in $CMDLINE; do
        case $x in
        cluster=false)
		echo "cluster disabled"
		exit 0;
                ;;
        esac
done

case "$1" in
  start)
        export LD_LIBRARY_PATH=/nexell/cluster
        export XDG_RUNTIME_DIR=/run/user/0
        /nexell/cluster/TiaPlayer.elf -w1920 -h720 -c/nexell/cluster/ -r/nexell/cluster/
  ;;

  stop)
        echo "Stopping Weston"
        killproc TiaPlayer.elf
  ;;

  restart)
	$0 stop
        sleep 1
        $0 start
  ;;

  *)
        echo "usage: $0 { start | stop | restart }"
  ;;
esac

exit 0
