#include <string>

#include <Arduino.h>

#include "DHT.h"

#include <ArduinoMqttClient.h>

#include <ESPAsyncWebServer.h>

#include <ESP8266WiFi.h>

#include "credentials.h"
#include "website.h"

AsyncWebServer server(80);
DHT           dht(5, DHT22);


WiFiClient    wifiClient;
MqttClient    mqttClient(wifiClient);

const char    broker[] = "192.168.0.11";
int           port     = 1883;
const char    topic[]  = "arduino/simple";

const long    interval = 1000;
unsigned long previousMillis = 0;

unsigned long prev_exec_time = 0;
unsigned long current_exec_time = 0;

float         temp = 0.0f;
float         humid = 0.0f; 
char          sensor_data[100];


const char* data = "{ \"hostname\":\"esphost\", \"wifi-ssid\":\"mySSID\", \"wifi-password\":\"mywifi-pass\", \"broker-address\":\"broker-address\", \"broker-user\":\"broker-user\", \"broker-password\":\"broker-password\", \"broker-update-interval\":1000, \"broker-topic\":\"myTopic\" }";

void setup() {
  snprintf(sensor_data, 100, "{\"temperature\":\"%.2f\",\"humidity\":\"%.2f\"}", 0.0, 0.0);

  dht.begin();
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println();
  Serial.println();
  Serial.println("Starting node...");
  Serial.println();
  Serial.println();



  WiFi.begin(ssid, pass);
  uint8_t status;
  while ((status = WiFi.status()) != WL_CONNECTED) {
        switch(status) {
        case STATION_GOT_IP:
            Serial.println("STATION_GOT_IP");
        case STATION_NO_AP_FOUND:
            Serial.println("STATION_NO_AP_FOUND");
        case STATION_CONNECT_FAIL:
            Serial.println("STATION_CONNECT_FAIL");
        case STATION_WRONG_PASSWORD:
            Serial.println("STATION_WRONG_PASSWORD");
        case STATION_IDLE:
            Serial.println("STATION_IDLE");
        default:
            Serial.println("DISCONNECTED");
    }
    
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Connected IP [");
  Serial.print(WiFi.localIP());
  Serial.println("]");
  

  mqttClient.setUsernamePassword(MQTTUSER, MQTTPASS);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  delay(1000);
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  server.on("/index.html", HTTP_GET, [] (AsyncWebServerRequest *request) { request->send(200, "text/html", index_html); });
  server.on("/style_index.css", HTTP_GET, [] (AsyncWebServerRequest *request) { request->send(200, "text/css", index_css); });
  server.on("/index.js", HTTP_GET, [] (AsyncWebServerRequest *request) { request->send(200, "text/jscript", index_js); });

  server.on("/config.html", HTTP_GET, [] (AsyncWebServerRequest *request) { request->send(200, "text/html", config_html); });
  server.on("/style_config.css", HTTP_GET, [] (AsyncWebServerRequest *request) { request->send(200, "text/css", config_css); });
  server.on("/config.js", HTTP_GET, [] (AsyncWebServerRequest *request) { request->send(200, "text/jscript", config_js); });

  server.on("/sensors", HTTP_GET, [] (AsyncWebServerRequest *request) { request->send(200, "text/plain", sensor_data); });

  server.on("/config", HTTP_GET, [] (AsyncWebServerRequest *request) { request->send(200, "text/plain", data); });
  server.on("/config", HTTP_POST, [] (AsyncWebServerRequest *request) { request->send(200, "application/json", ""); });


  server.onNotFound([] (AsyncWebServerRequest *request) { request->send(404, "text/html", "Ressource not found"); });

  prev_exec_time = millis();
  current_exec_time = millis();

  server.begin();
}



void loop() {

  current_exec_time = millis();

  if (current_exec_time - prev_exec_time < 5000)
  {
    delayMicroseconds(100000);
    return;
  }
  prev_exec_time = millis();

  temp = dht.readTemperature();
  humid = dht.readHumidity();
  snprintf(sensor_data, 100, "{\"temperature\":\"%.2f\",\"humidity\":\"%.2f\"}", temp, humid);

  Serial.println("Loop");

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("\tHumidity: ");
  Serial.println(humid);

  mqttClient.poll();

    // save the last time a message was sents
  Serial.print("Sending message to topic: ");
  Serial.println(topic);

  mqttClient.beginMessage(topic);
  mqttClient.print("temp ");
  mqttClient.print(temp);
  mqttClient.endMessage();

}

	// WiFi.begin("Tardis", "Thisisavery4312longpassword$#!@");
