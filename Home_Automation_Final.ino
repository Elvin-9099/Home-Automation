#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID ""
#define WIFI_PASS ""

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME ""
#define MQTT_PASS ""

int b1 = 2;
int b2 = 3;
int tv = 4;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe BulbOne = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/BulbOne");
Adafruit_MQTT_Subscribe BulbTwo = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/BulbTwo");
Adafruit_MQTT_Subscribe TV = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/TV");



void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);


  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi>");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  digitalWrite(LED_BUILTIN, LOW);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(">");
    delay(50);
  }

  Serial.println("OK!");

  //Subscribe to the onoff topic
  mqtt.subscribe(&onoff);

  pinMode(b1, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(b1, LOW);
  pinMode(b2, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(b2, LOW);
  pinMode(tv, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(tv, LOW);

}

void loop()
{
  //Connect/Reconnect to MQTT
  MQTT_connect();

  
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &BulbOne)
    {
      if (!strcmp((char*) BulbOne.lastread, "ON"))
      {
        digitalWrite(b1, HIGH);
      }
      else if(!strcmp((char*) BulbOne.lastread, "OFF"))
      {
        digitalWrite(b1, LOW);
      }
    }
    else if (subscription == &BulbTwo)
    {
      if (!strcmp((char*) BulbTwo.lastread, "ON"))
      {
        digitalWrite(b2, HIGH);
      }
      else if(!strcmp((char*) BulbTwo.lastread, "OFF"))
      {
        digitalWrite(b2, LOW);
      }
    }
    else if (subscription == &TV)
    {
      if (!strcmp((char*) TV.lastread, "ON"))
      {
        digitalWrite(TV, HIGH);
      }
      else if(!strcmp((char*) TV.lastread, "OFF"))
      {
        digitalWrite(TV, LOW);
      }
}


void MQTT_connect()
{

  
  if (mqtt.connected() && mqtt.ping())
  {
    //    mqtt.disconnect();
    return;
  }

  int8_t ret;

  mqtt.disconnect();

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) 
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      ESP.reset();
    }
  }
  Serial.println("MQTT Connected!");
}
