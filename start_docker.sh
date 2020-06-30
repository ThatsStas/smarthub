#!/bin/bash

# NodeRed
#"Env": [
#                "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
#                "NODE_VERSION=10.20.1",
#                "YARN_VERSION=1.22.4",
#                "NODE_RED_VERSION=v1.0.6",
#                "NODE_PATH=/usr/src/node-red/node_modules:/data/node_modules",
#                "FLOWS=flows.json"
#            ],
#
#
#
#
#  Mosquitto
#"Env": [
#                "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
#                "VERSION=1.6.9",
#                "DOWNLOAD_SHA256=412979b2db0a0020bd02fa64f0a0de9e7000b84462586e32b67f29bb1f6c1685",
#                "GPG_KEYS=A0D6EEA1DCAE49A635A3B2F0779B22DFB3E717B7",
#                "LWS_VERSION=2.4.2"
#            ],




docker run -d -it -p 1883:1883 -p 9001:9001 -v /home/pi/docker/mqtt/:/mosquitto --name mqtt  eclipse-mosquitto

if [[ $? != 0 ]]; then
    docker restart mqtt
    
fi

docker run -d -it --group-add dialout -e TZ=Europe/Berlin -p 1880:1880 -v /home/pi/docker/nodered:/data --name nodered nodered/node-red

if [[ $? != 0 ]]; then
    docker restart nodered
    
fi

