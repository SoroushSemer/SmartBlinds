/* MQTT SMART BLINDS NODE RED CONNECTION
 * CODE WRITTEN BY: SOROUSH SEMERKANT
 * LINKEDIN: https://www.linkedin.com/in/soroush-semerkant/
 * GITHUB: https://github.com/SoroushSemer
 * PUBLISHED ON: 6/8/2021
 */

#include <ESP8266WiFi.h>                                                  //ESP8266 Wifi Library
#include <PubSubClient.h>                                                 //MQTT Library 

const char* ssid = "SSID";                                                //wifi ssid for connection
const char* wifi_password = "PASSWORD";                                   //wifi password for connection

const char* mqtt_server = "IP ADDRESS";                                   //MQTT server address
const char* mqtt_topic = "right_blind";                                   //MQTT topic to subscribe to for the blinds
const char* mqtt_username = "USERNAME";                                   //MQTT login username
const char* mqtt_password = "PASSWORD";                                   //MQTT login password
const char* clientID = "ESP8266_3";                                       //Device ID to display to MQTT server

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); 

int espOutput1 = D0;                                                      //pin definirion for esp to arduino connection
int espOutput2 = D1;                                                      //pin definition for esp to arduion connection

int updown = 1;                                                           //blinds state starts down 
String msg;

void MQTTMessage(char* topic, byte* payload, unsigned int length) {       //Messaged Recieved from MQTT Server
  Serial.print((char)payload[15]);                                        //Prints 3 characters indicating blinds percentage
  Serial.print((char)payload[16]);
  Serial.println((char)payload[17]);
  if ((char)payload[15] == '0') {                                         //if the first digit place is 0 (0%)
    digitalWrite(espOutput1, 1);                                          //go up
    digitalWrite(espOutput2, 0);
    delay(300);
    digitalWrite(espOutput1, 0);
    digitalWrite(espOutput2, 0);
  }
  else if ((char)payload[15] == '1') {                                    //if the first digit place is 1 (100%)
    digitalWrite(espOutput1, 0);                                          //go down
    digitalWrite(espOutput2, 1);
    delay(300);
    digitalWrite(espOutput1, 0);
    digitalWrite(espOutput2, 0);
  }
  else {                                                                  //if the first digit place is anything else (1-99%)
    digitalWrite(espOutput1, 1);                                          //go half
    digitalWrite(espOutput2, 1);
    delay(300);
    digitalWrite(espOutput1, 0);
    digitalWrite(espOutput2, 0);
  }


}

bool Connect() {
  if (client.connect(clientID, mqtt_username, mqtt_password)) {            //Connects to the MQTT server with the given ID username and password
    client.subscribe(mqtt_topic);                                          //Subscribes to the blinds topic
    return true;
  }
  else {
    return false;                                                           //Sends false if the connection failed
  }
}

void setup() {
  pinMode(espOutput1,OUTPUT);                                               //initializes esp to arduino output pins
  pinMode(espOutput2,OUTPUT);
  Serial.begin(115200);                                                     //Tyrns on Serial Monitor to 115200 baud rate

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, wifi_password);                                          //Connects to WiFi

  while (WiFi.status() != WL_CONNECTED) {                                   //Checks status of WiFi connection
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());                                           //Prints IP address of the module

  client.setCallback(MQTTMessage);
  if (Connect()) {                                                          //Checks if the device was able to connect to the MQTT server
    Serial.println("Connected Successfully to MQTT Broker!");
  }
  else {
    Serial.println("Connection Failed!");
  }
}

void loop() {
  if (!client.connected()) {                                                //Checks if the device is not connected to the MQTT server and connects it
    Connect();
  }
  client.loop();
}
