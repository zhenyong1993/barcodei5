#!/bin/bash

#kill barcode
ps -ef | grep "barcode$" | egrep -v "grep|$0" | awk '{print $2}' | xargs kill -9

#delete barcode
APPHEAD="/home/isesol/nut/app/toolapply"
rm -rf $APPHEAD

echo "toolapply uninstall success"
