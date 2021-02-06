#include <Arduino.h>

#include "DHT.h"

#include <ArduinoMqttClient.h>

#include <ESPAsyncWebServer.h>

#include <ESP8266WiFi.h>

#include "ArduinoJson.h"

#include "credentials.h"
#include "website.h"
#include "data_handler.h"

DataHandler *dataHandler;
AsyncWebServer server(80);
DHT dht(5, DHT22);

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const int MAX_RETRY = 10;
bool isConnected = false;

int port = 1883;
char *topic;
String tempTopic, humidTopic;

uint32 updateInterval = 1000;
unsigned long previousMillis = 0;

unsigned long prev_exec_time = 0;
unsigned long current_exec_time = 0;

float temp = 0.0f;
float humid = 0.0f;
char sensor_data[100];

IPAddress local_IP(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 254);
IPAddress subnet(255, 255, 255, 0);

void setup()
{
  Serial.begin(115200);

  dataHandler = new DataHandler();
  topic = dataHandler->brokerTopic();

  snprintf(sensor_data, 100, "{\"temperature\":\"%.2f\",\"humidity\":\"%.2f\"}", 0.0, 0.0);

  dht.begin();
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println();
  Serial.println();
  Serial.println("Starting node...");
  Serial.println();
  Serial.println();

  int retryCounter = 0;
  WiFi.begin(dataHandler->wifiSSID(), dataHandler->wifiPass());

  uint8_t status;
  while ((status = WiFi.status()) != WL_CONNECTED)
  {
    switch (status)
    {
    case STATION_GOT_IP:
      Serial.println("STATION_GOT_IP");
      break;
    case STATION_NO_AP_FOUND:
      Serial.println("STATION_NO_AP_FOUND");
      break;
    case STATION_CONNECT_FAIL:
      Serial.println("STATION_WRONG_PASSWORD");
      break;
    case STATION_IDLE:
      Serial.println("STATION_IDLE");
      break;
    default:
      Serial.println("DISCONNECTED");
      break;
    }

    if (retryCounter >= MAX_RETRY)
      break;

    retryCounter++;
    Serial.print(".");
    delay(1000);
  }
  Serial.print("Retry counter: ");
  Serial.println(retryCounter);

  if (retryCounter >= MAX_RETRY)
  {
    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

    Serial.print("Setting up Soft AP ... ");
    Serial.println(WiFi.softAP("ESP_NODE_WiFi", "Vopop=Upolu331", 3, 0, 2) ? "Ready" : "Failed");
    Serial.print("Setup complete: \n\tSSID: ESP_NODE_WIFI\n\tpassword: Vopop=Upolu331\n\tIP: ");
    Serial.println(WiFi.softAPIP());
    // WiFi.begin();
  }
  else
  {
    updateInterval = dataHandler->updateInterval();
    isConnected = true;
  }

  Serial.print("Connected IP [");
  Serial.print(WiFi.localIP());
  Serial.println("]");

  if (isConnected)
  {
    mqttClient.setUsernamePassword(dataHandler->brokerUser(), dataHandler->brokerPassword());
    if (!mqttClient.connect(dataHandler->brokerAddress(), port))
    {
      Serial.print("MQTT connection failed! Error code = ");
      Serial.println(mqttClient.connectError());

      // while (1);
    }
    delay(1000);
    Serial.println("You're connected to the MQTT broker!");
    Serial.println();
  }

  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/html", index_html); });

  server.on("/style_index.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/css", index_css); });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/jscript", index_js); });

  server.on("/config.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/html", config_html); });

  server.on("/style_config.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/css", config_css); });

  server.on("/config.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/jscript", config_js); });

  server.on("/sensors", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/plain", sensor_data); });

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/plain", dataHandler->getSerializedJson()); });

  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    Serial.println("=======================");
    Serial.println("=======================");
    Serial.print("Len: ");
    Serial.println(len);
    Serial.print("Index: ");
    Serial.println(index);
    Serial.print("Total: ");
    Serial.println(total);
    Serial.print("Data: ");
    Serial.println((char *)data);

    static char *rawJson = nullptr;
    if (index == 0)
    {
      if (rawJson != nullptr)
        delete rawJson;

      rawJson = (char *)malloc(total * sizeof(char));
    }

    // copy first chunk if data is not completely transmitted
    if (index == 0 && len != total)
    {
      memcpy(rawJson, data, len);
      return;
    }
    memcpy(rawJson + index, data, len);

    // Remove escaped \" and first and last " from the json string coming from the frontend
    String s = rawJson;
    s.replace("\\\"", "\"");
    s = s.substring(1, s.length() - 1);

    memcpy(rawJson, s.c_str(), s.length());

    StaticJsonDocument<1024> buf;
    deserializeJson(buf, rawJson);

    Serial.print("Raw JSON: ");
    Serial.println(rawJson);
    Serial.print("Json buffer: ");
    Serial.println(buf.data().asString());

    JsonObject obj = buf.as<JsonObject>();
    Serial.print("Setting data with object size: ");
    Serial.println(obj.size());

    dataHandler->setData(obj);
  });

  server.onNotFound([](AsyncWebServerRequest *request) {});

  prev_exec_time = millis();
  current_exec_time = millis();

  server.begin();

  tempTopic = topic;
  tempTopic += "/temperature";

  humidTopic = topic;
  humidTopic += "/humidity";
}

void loop()
{

  if (!isConnected)
  {
    delayMicroseconds(1000);
    return;
  }

  current_exec_time = millis();

  if (current_exec_time - prev_exec_time < updateInterval)
  {
    delayMicroseconds(10000);
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

  mqttClient.beginMessage(tempTopic);
  mqttClient.print(temp);
  mqttClient.endMessage();

  mqttClient.beginMessage(humidTopic);
  mqttClient.print(humid);
  mqttClient.endMessage();
}