#!/bin/bash

DOCKER_PATH=$(pwd)

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
#
#
#
# InfluxDB
# https://hub.docker.com/_/influxdb





#setup_mqtt() {
#    [[ -f $DOCKER_PATH/mqtt/passwd ]] && return;
#    
#    [[ $EUID != 0 ]] && { echo "EE: Please run this script a sudo"; exit 1; }
#
#    local mqtt_username 
#    local mqtt_password 
#    echo "Setup mosquitto mqtt broker"
#    read -p "Username: " mqtt_username 
#    read -sp "Password: " mqtt_password
#
#    read -p "Press CTL+c to abort or ENTER to continue installation"
#
#    [[ ! -d $DOCKER_PATH/mqtt ]] && { echo "EE: No MQTT config path available at $DOCKER_PATH/mqtt. Exiting..."; exit 1; }
#
#    sudo echo $mqtt_username:$mqtt_password >> $DOCKER_PATH/mqtt/passwd
#
#    docker run -d --rm -p 1884:1883 -p 9002:9001 -v $DOCKER_PATH/mqtt/:/mosquitto --name mqtt  eclipse-mosquitto:1.6 /bin/sh -c "/usr/bin/mosquitto_passwd -U /mosquitto/passwd"
#    sleep 10
#}
#
#
#setup_mqtt
##docker run -d --restart unless-stopped -p 1883:1883 -p 9001:9001 -v $DOCKER_PATH/mqtt/:/mosquitto --name mqtt  eclipse-mosquitto:1.6
#
#if [[ $? != 0 ]]; then
#    docker restart mqtt
#    
#fi
#


[[ $# > 0 ]] && { [[ $1 == "stop" ]] && echo "Stopping all containers"; docker stop grafana nodered influxdb; docker rm grafana nodered influxdb; exit 1; }


[[ ! -d $DOCKER_PATH/nodered ]] && mkdir -p $DOCKER_PATH/nodered

FOLDER_OWNER=$(stat -c '%u' $DOCKER_PATH/nodered)
EFFECTIVE_UID=$(id -u)
EFFECTIVE_GUID=$(id -g)

[[ "$FOLDER_OWNER" != "$EFFECTIVE_UID" ]] && { sudo chown -R $EFFECTIVE_UID:$EFFECTIVE_GUID $DOCKER_PATH/nodered; }
docker run --restart unless-stopped -d --group-add dialout -e TZ=Europe/Berlin -p 1880:1880 -v $DOCKER_PATH/nodered:/data --name nodered nodered/node-red:1.1.3

if [[ $? != 0 ]]; then
    docker restart nodered
    
fi

docker run --restart unless-stopped -d -p 8086:8086 -p 2003:2003 -e INFLUXDB_ADMIN_USER=admin -e INFLUXDB_ADMIN_PASSWORD=mypassword -e INFLUXDB_GRAPHITE_ENABLED=true -v $DOCKER_PATH/influx:/var/lib/influxdb --name=influxdb influxdb:1.8.2

[[ ! -f $DOCKER_PATH/grafana/grafana.ini ]] && touch $DOCKER_PATH/grafana/grafana.ini


# TODO: Remove password from environment list.
docker run --restart unless-stopped -d -v $DOCKER_PATH/grafana/config:/etc/grafana -v $DOCKER_PATH/grafana/data:/var/lib/grafana -v $DOCKER_PATH/grafana/log:/var/log/grafana -v $DOCKER_PATH/grafana/grafana.ini:/etc/grafana/grafana.ini -p 3000:3000 --name=grafana -u 104 grafana/grafana:7.1.5

