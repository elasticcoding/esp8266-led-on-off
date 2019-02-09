#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
 
#define LED 2
 
//Enter your wifi credentials
const char* ssid = "OPENCLOUDS";  
const char* password =  "1234567889";
 
//Enter your mqtt server configurations
const char* mqttServer = "live.openclouds.in";    //Enter Your mqttServer address
const int mqttPort = 1883;       //Port number
//const char* mqttUser = "asdkrtevaoq"; //User
//const char* mqttPassword = "sfCJ8Rafbzd"; //Password
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  delay(1000);
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(MQTTcallback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266")) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.println(client.state());  //If you get state 5: mismatch in configuration
      delay(2000);
 
    }
  }
 
  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");
 
}
 
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
 
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];  //Conver *byte to String
  }
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(message);
      const char* led = root["led"];
      const char* stat = root["status"];
      String led_stat((const __FlashStringHelper*) stat);

      Serial.println(led);
      //Serial.println(stat);
   Serial.print(message);
  if(led_stat == "#on") {client.publish("esp/test", "light is on"); client.subscribe("esp/test");digitalWrite(LED,LOW);}   //LED on  
  if(led_stat == "#off") {client.publish("esp/test", "light is off");client.subscribe("esp/test");digitalWrite(LED,HIGH);} //LED off
 
  Serial.println();
  Serial.println("-----------------------");  
}
 
void loop() {
  client.loop();
}
