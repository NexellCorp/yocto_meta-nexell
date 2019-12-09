#!/bin/sh

### Wayland Environment Variables ###

export XDG_CONFIG_HOME=/etc/
export XDG_RUNTIME_DIR=/tmp/wayland-runtime-dir
export SDL_VIDEODRIVER=wayland
export WAYLAND_DISPLAY=wayland-0
export WS_CALUDEV_FILE=/etc/udev/rules.d/ws-calibrate.rules
export QT_QPA_EGLFS_INTEGRATION=eglfs_kms

if ! test -d "${XDG_RUNTIME_DIR}"; then
       mkdir -p "${XDG_RUNTIME_DIR}"
       chmod 0700 "${XDG_RUNTIME_DIR}"
fi
