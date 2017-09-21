#!/bin/bash

set -e

declare -a targets=("qt3d" "qtbase" "qtconnectivity" "qtdeclarative" "qtenginio" "qtimageformats" "qtgraphicaleffects" "qtlocation" "qtmultimedia" "qtquick1" "qtquickcontrols" "qtscript" "qtsensors" "qtserialport" "qtsvg" "qtsystems" "qttools" "qttranslations" "qtwayland" "qtwebchannel" "qtwebkit-examples" "qtwebengine" "qtwebkit" "qtwebsockets" "qtx11extras" "qtxmlpatterns")

function compress()
{
    for i in ${targets[@]}
    do
        echo $i
        mv $i git
        tar cvzf $i.tar.gz git
        mv git $i
    done
}

compress

