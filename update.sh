#!/bin/bash

#check command zip
if command -v zip > /dev/null 2>&1; then
echo "zip installed"
else
sudo apt-get install -y zip
fi

#check lib uuid
if [ `dpkg -l | grep uuid-dev | wc -l` -eq 0 ]; then
sudo apt-get install -y uuid-dev
else
echo "uuid installed"
fi

APPHEAD=/home/isesol/nut/app/toolapply
APPBIN=$APPHEAD/bin
APPCONFIG=$APPHEAD/config
APPDATA=$APPHEAD/data
DYNAMICCONFIG=/home/isesol/config/structure

DIR=$(cd "$(dirname "$0")";pwd)

#kill dwh
echo "kill dwh ..."
ps -ef | grep "dwh$" | egrep -v "grep|$0" | awk '{print $2}' | xargs kill -9

#delete dwh
if [ -d $APPBIN ];then
echo "delete $APPBIN ..."
rm -rf $APPBIN
fi

if [ -d $APPCONFIG ];then
echo "delete $APPCONFIG ..."
rm -rf $APPCONFIG
fi

if [ -d $APPDATA ];then
echo "delete $APPDATA ..."
rm -rf $APPDATA
fi

#create dwh
echo "create $APPBIN ..."
mkdir $APPBIN

echo "create $APPCONFIG ..."
mkdir $APPCONFIG

echo "create $APPDATA ..."
mkdir $APPDATA
mkdir $APPDATA/log
mkdir $APPDATA/file


#update dwh
echo "update dwh ..."
cp -r $DIR/barcode $APPBIN/
cp -r $DIR/lib* $APPBIN/
cp -r $DIR/desktop $APPHEAD/
chmod +x $APPBIN/barcode

echo "update success"
