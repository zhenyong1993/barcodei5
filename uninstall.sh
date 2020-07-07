#!/bin/bash

#kill dwh
ps -ef | grep "dwh$" | egrep -v "grep|$0" | awk '{print $2}' | xargs kill -9

#delete dwh
APPHEAD="/home/isesol/nut/app/dwh"
rm -rf $APPHEAD

echo "dwh uninstall success"
