#!/bin/sh

appname=`basename "$0" | sed s,\.sh$,,`
dirname=`dirname "$0"`
tmp="${dirname#?}"
if [ "${dirname%$tmp}" != "/" ]; then
dirname=$PWD/$dirname
fi

LD_LIBRARY_PATH=${dirname}/libs
export LD_LIBRARY_PATH
sed -i "s,Icon=.*,Icon=${dirname}/bial-logo.png,g" "$dirname/../bialgui.desktop"
"$dirname/$appname" "$@"
