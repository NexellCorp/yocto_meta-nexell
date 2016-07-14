#/bin/sh

export XDG_RUNTIME_DIR="/run/shm/wayland"
mkdir -p "$XDG_RUNTIME_DIR"
chmod 0700 "$XDG_RUNTIME_DIR"

/usr/bin/weston --backend=drm-backend.so --tty=1 --log=/var/log/weston.log
