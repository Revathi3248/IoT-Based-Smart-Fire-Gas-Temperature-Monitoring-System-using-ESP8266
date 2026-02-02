#include <ESP8266WiFi.h>

#include "Adafruit_MQTT.h"

#include "Adafruit_MQTT_Client.h"

 

#include "DHT.h"

#define DHTPIN D2   // Digital pin connected to the DHT sensor
#define flame D1
#define gas A0
#define pump D5
#define fan D3
int  buzzer =D7;
int  led= D0;




#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

#define WLAN_SSID       "vivo V29"

#define WLAN_PASS       "curly@488"

#define MQTT_SERV      "io.adafruit.com"

#define MQTT_PORT  1883

#define MQTT_NAME  "pro_project_000"         

#define MQTT_PASS  "aio_QKsn35MY1T1UCMMotOnZoCYTrRZD"
////////////////////////////////// Feeds  ///////////////////////////////////////////////////////////////////////////

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Publish tempfeed = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/temp");

Adafruit_MQTT_Publish humfeed = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/hum");

Adafruit_MQTT_Publish flamefeed = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/flame");

Adafruit_MQTT_Publish gasfeed = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/gas");

 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 

void setup() {

Serial.begin(9600);

Serial.print("connecting to ");

dht.begin();

 

pinMode(flame,INPUT); // put your setup code here, to run once:
pinMode(gas,INPUT);
pinMode(buzzer,OUTPUT);
pinMode(led,OUTPUT);
pinMode(fan,OUTPUT);
pinMode(pump,OUTPUT);

digitalWrite(fan,HIGH);
digitalWrite(pump,HIGH);
digitalWrite(led,LOW);


Serial.println(WLAN_SSID);

WiFi.begin(WLAN_SSID, WLAN_PASS);

while (WiFi.status() != WL_CONNECTED)

  {

    delay(500);

    Serial.print(".");

  }

Serial.println();

Serial.println("WiFi connected");

Serial.println("IP address: ");

Serial.print(WiFi.localIP());

digitalWrite(buzzer,HIGH);
delay(500);
digitalWrite(buzzer,LOW);
MQTT_connect();

}

void loop() {

 

  int t = dht.readTemperature();

  int h = dht.readHumidity();

  int flamevalue = digitalRead(flame);

  int gasvalue = analogRead(gas);

 

Serial.println(t);
Serial.println(h);
Serial.println(flamevalue);
Serial.println(gasvalue);

if (tempfeed.publish(t)){

  Serial.print("updated.....");

  delay(2000);

}

 

if (humfeed.publish(h)){

  Serial.print("updated.....");

  delay(2000);

}

if (flamefeed.publish(flamevalue)){

  Serial.print("updated.....");

  delay(2000);

}

if (gasfeed.publish(gasvalue)){

  Serial.print("updated.....");

  delay(2000);

}





if(flamevalue == 0){

  digitalWrite(pump,LOW);
  delay(100);

  digitalWrite(led,HIGH);
  delay(500);
  digitalWrite(led,LOW);
  delay(100);

}

 

else{

    digitalWrite(pump,HIGH);

 

}

 

if ((gasvalue > 300) || ((t > 28) && ((t < 85)))){

  digitalWrite(fan,LOW);
  digitalWrite(buzzer,HIGH);
  delay(1000);
  digitalWrite(buzzer,LOW);

}

else{

    digitalWrite(fan,HIGH);

 

}

 

 }

void MQTT_connect()

{

  int8_t ret;

  if (mqtt.connected())

{

  return;

 

}

 

 Serial.print("Connecting to MQTT... ");

 

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0)

    {

       Serial.println(mqtt.connectErrorString(ret));

       Serial.println("Retrying MQTT connection in 5 seconds...");

       mqtt.disconnect();

       delay(5000);

       retries--;

       if (retries == 0)

       {

         while (1);

       }

    }

  Serial.println("MQTT Connected!");

}




 

 

 

