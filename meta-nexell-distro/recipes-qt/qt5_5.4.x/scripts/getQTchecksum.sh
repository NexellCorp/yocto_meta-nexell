#!/bin/bash

set -e

declare -a targets=("qt3d" "qtbase" "qtconnectivity" "qtdeclarative" "qtenginio" "qtimageformats" "qtgraphicaleffects" "qtlocation" "qtmultimedia" "qtquick1" "qtquickcontrols" "qtscript" "qtsensors" "qtserialport" "qtsvg" "qtsystems" "qttools" "qttranslations" "qtwayland" "qtwebchannel" "qtwebkit-examples" "qtwebengine" "qtwebkit" "qtwebsockets" "qtx11extras" "qtxmlpatterns")

function getchecksum()
{
    for i in ${targets[@]}
    do
        echo $i
        local md5sumStr=`md5sum $i.tar.gz | cut -d ' ' -f 1`
        local sha256sumStr=`sha256sum $i.tar.gz | cut -d ' ' -f 1`
        echo "SRC_URI]md5sum] = \"$md5sumStr\""
        echo "SRC_URI]sha256sum] = \"$sha256sumStr\""
    done
}


getchecksum
