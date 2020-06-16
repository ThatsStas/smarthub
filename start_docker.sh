#!/bin/bash


docker run -d -it -p 1883:1883 -p 9001:9001 -v /home/pi/docker/mqtt/:/mosquitto --name mqtt  eclipse-mosquitto

if [[ $? != 0 ]]; then
    docker restart mqtt
    
fi

docker run -d -it --group-add dialout -e TZ=Europe/Berlin -p 1880:1880 -v /home/pi/docker/nodered:/data --name nodered nodered/node-red

if [[ $? != 0 ]]; then
    docker restart nodered
    
fi

