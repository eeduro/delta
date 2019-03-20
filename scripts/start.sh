#!/bin/bash
echo -e "\e[92m+++++++++++++++Starting eeduro-delta application++++++++++++++++$

mouse="/dev/input/event1"

sleep 20

if [ -c "$mouse" ]
then
        echo "mouse found"
        sudo screen -dmS mouse cat /dev/input/event1
else
        echo "no mouse connected"
fi

/usr/bin/sudo /opt/eeros/bin/myApp -c /opt/eeros/bin/HwConfigBBBlue.json
/usr/bin/sudo halt

