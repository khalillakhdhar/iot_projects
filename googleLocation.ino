
#include <Arduino.h>
#ifdef ARDUINO_ARCH_SAMD
#include <WiFi101.h>
#elif defined ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#error Wrong platform
#endif 

#include <WifiLocation.h>

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "BBFF-um5IfjczgSEQtUv08mg0yCKuAyz9cA" // Your Ubidots TOKEN
#define WIFINAME "globalnet" //Your SSID
#define WIFIPASS "darna2018" // Your Wifi Pass
Ubidots client(TOKEN);
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

const char* googleApiKey = "AIzaSyCs-JZX-KXSb-DwIL3oI61ShE4r4GSlj84";
const char* ssid = "globalnet";
const char* passwd = "darna2018";

WifiLocation location(googleApiKey);

void setup() {
  
   Serial.begin(115200);
client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);

 client.begin(callback);
    // Connect to WPA/WPA2 network
#ifdef ARDUINO_ARCH_ESP32
    WiFi.mode(WIFI_MODE_STA);
#endif
#ifdef ARDUINO_ARCH_ESP8266
    WiFi.mode(WIFI_STA);
#endif
    WiFi.begin(ssid, passwd);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        // wait 5 seconds for connection:
        Serial.print("Status = ");
        Serial.println(WiFi.status());
        delay(500);
    }
   
}

void loop() {
 location_t loc = location.getGeoFromWiFi();
/*
 * float value = 1;
  char context[25];
  sprintf(context, "lat=1.2343$lng=132.1233"); //Sends latitude and longitude for watching position in a map
  client.add(VARIABLE_LABEL, value, context);  // Change for your variable name
  
  if(client.send()){
   Serial.println("Data sent to Ubidots sucessfully!")
  }

  delay(5000);
 * 
 * 
 */
  float value = 1;
  char context[25];
    Serial.println("Location request data");
    Serial.println(location.getSurroundingWiFiJson());
    Serial.println("Latitude: " + String(loc.lat, 7));
    Serial.println("Longitude: " + String(loc.lon, 7));
    Serial.println("Accuracy: " + String(loc.accuracy));
   // sprintf(context,"lat=%f$lng=%f",loc.lat,loc.lon);
   sprintf(context, "lat=1.2343$lng=132.1233");
 client.add("position", value, context);  // Change for your variable name
  client.ubidotsPublish("cas2");
  
  delay(2000);
 
}
