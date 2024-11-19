#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include  <string.h>
#include "my_uart.h"
#define STASSID "pass tu mot den chin"
#define STAPSK "mothaiba"


/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "kido2k3"
#define AIO_KEY         "aio_LGpV02LhvdeooxzT5cXJeds114FG"
// setup MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client , AIO_SERVER , AIO_SERVERPORT , AIO_USERNAME , AIO_KEY);
// set publish
Adafruit_MQTT_Publish light_pub = Adafruit_MQTT_Publish (& mqtt , AIO_USERNAME "/feeds/led");
Adafruit_MQTT_Publish temp_pub = Adafruit_MQTT_Publish (& mqtt , AIO_USERNAME "/feeds/iot-temperature");
// set subcribe
Adafruit_MQTT_Subscribe light_sub = Adafruit_MQTT_Subscribe (&mqtt , AIO_USERNAME "/feeds/led", MQTT_QOS_1);
Adafruit_MQTT_Subscribe temp_sub = Adafruit_MQTT_Subscribe (&mqtt , AIO_USERNAME "/feeds/iot-temperature", MQTT_QOS_1);
int led_counter = 0;
int led_st = 1;

void lightcallback(char* value , uint16_t len) {
  if (value [0] == '0') Serial.print('a');
  if (value [0] == '1') Serial.print('A');
}
void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  // test connection to wifi
  digitalWrite(5, 1);
  Serial.begin (115200);
  // connect Wifi
  WiFi.begin(STASSID , STAPSK);
  while (WiFi.status () != WL_CONNECTED) {
    delay (500);
  }
  // subscribe light feed
  light_sub.setCallback(lightcallback);
  mqtt.subscribe (&light_sub);
  //connect to server
  while (mqtt.connect () != 0) {
    mqtt.disconnect ();
    delay (500);
  }
  digitalWrite(5, 0);
}

int state = 0;


void loop() {
  // put your main code here, to run repeatedly:
  // receive packet
  mqtt.processPackets (1);
  double temp_data = 0;  
  uint8_t success = 0;
  if (Serial.available()) {
    int msg = Serial.read();
    get_data(msg);
  }
  success = compile_cmd(&temp_data);
  if(success == 1) {
    temp_pub.publish(temp_data);
  }
  led_counter++;
  if (led_counter == 100) {
    if (led_st == 1) led_st = 0;
    else led_st = 1;
    digitalWrite(2, led_st);
    led_counter = 0;
  }
  delay(10);
}
