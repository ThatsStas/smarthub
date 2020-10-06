

#include <Arduino.h>

#include "DHT.h"

#include <ArduinoMqttClient.h>

#include <ESPAsyncWebServer.h>

#include <ESP8266WiFi.h>

#include "credentials.h"
#include "website.h"

AsyncWebServer server(80);
AsyncWebServer s_server(443);

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.0.11";
int        port     = 1883;
const char topic[]  = "arduino/simple";

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;

const char* data = "{ \"hostname\":\"esphost\", \"wifi-ssid\":\"mySSID\", \"wifi-password\":\"mywifi-pass\", \"broker-address\":\"broker-address\", \"broker-user\":\"broker-user\", \"broker-password\":\"broker-password\", \"broker-update-interval\":1000, \"broker-topic\":\"myTopic\" }";

DHT dht(5, DHT22);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/html", "Not found");
}

void sendInfo(AsyncWebServerRequest *request) {
    request->send(200, "text/html", website.c_str());

}

void configureClient(AsyncWebServerRequest *request) {
  // request.
}

void setupWebServer() {
  server.on("/", HTTP_GET, sendInfo);
  server.on("/", HTTP_POST, configureClient);
  s_server.on("/", HTTP_GET, sendInfo);
  s_server.on("/", HTTP_POST, configureClient);
  s_server.on("/get_config", HTTP_GET, [&] (AsyncWebServerRequest *request) { request->send(200, "text/html", data);});

  server.on("/get_config", HTTP_GET, [&] (AsyncWebServerRequest *request) { request->send(200, "text/html", data);});
  server.on("/config.html", HTTP_GET, [&] (AsyncWebServerRequest *request) { request->send(200, "text/html", config_html.c_str());});
  server.on("/config.js", HTTP_GET, [&] (AsyncWebServerRequest *request) { request->send(200, "text/jscript", config_js.c_str());});  
  server.on("/style_config.css", HTTP_GET, [&] (AsyncWebServerRequest *request) { request->send(200, "text/css", config_css.c_str());});



  server.onNotFound(notFound);
  s_server.onNotFound(notFound);

  server.begin();
  s_server.begin();
}

void setup() {
  dht.begin();
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }



  Serial.println("Starting node...");

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

  Serial.print("Connected. IP [");
  Serial.print(WiFi.localIP());
  Serial.println("]");
  

  mqttClient.setUsernamePassword(MQTTUSER, MQTTPASS);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  delay(2);

  setupWebServer();
}



void loop() {
  auto temp = dht.readTemperature();
  auto humid = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("\tHumidity: ");
  Serial.println(humid);

  mqttClient.poll();

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    Serial.print("Sending message to topic: ");
    Serial.println(topic);

    mqttClient.beginMessage(topic);
    mqttClient.print("temp ");
    mqttClient.print(temp);
    mqttClient.endMessage();

    Serial.println();
  }
  delay(5000);
}

	// WiFi.begin("Tardis", "Thisisavery4312longpassword$#!@");
