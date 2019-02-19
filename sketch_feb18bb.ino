#include <Adafruit_MQTT_Client.h>
#include <Adafruit_MQTT.h>
#include <uTimerLib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <WiFi.h>

DHT dht(32, DHT11);

WiFiClient wiFiClient;
Adafruit_MQTT_Client mqttClient(&wiFiClient, "192.168.0.105", 1883);
Adafruit_MQTT_Publish temperaturePublisher(&mqttClient, "/temperature");
Adafruit_MQTT_Publish humidityPublisher(&mqttClient, "/humidity");

void sendData() {
  int temperature = dht .readTemperature();
  temperaturePublisher.publish(temperature);

  int humidity = dht.readHumidity();
  humidityPublisher.publish(humidity);
}

void setup() {
  dht.begin();
  Serial.begin(115200);
  WiFi.begin("createch2019", "createch2019");
  delay(5000);
  Serial.print(WiFi.localIP());
  TimerLib.setInterval_s(sendData, 2);

}

void loop() {
  if (mqttClient.connected()) {
    mqttClient.processPackets(10000);
    mqttClient.ping();
  } else {
    mqttClient.disconnect();
    mqttClient.connect();
  }

}
