#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h" 

const int led_Pin1 = 2;
const int led_Pin2 = 15;
const int led_Pin3 = 4;

bool array[8][3] = {
  {false,false,false},
  {false,false,true},
  {false,true,false},
  {false,true,true},
  {true,false,false},
  {true,false,true},
  {true,true,false},
  {true,true,true},
};


void setup() {
  Serial.begin(9600);
	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  pinMode(led_Pin1,OUTPUT);
  pinMode(led_Pin2,OUTPUT);
  pinMode(led_Pin3,OUTPUT);

	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
  
    // Establish a connection to the server
    http.begin(endpoint);
    
    for(int r=0; r<8; r++) {

       // Specify content-type header
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-API-Key", "Ralph#8500");

    // Serialise JSON object into a string to be sent to the API
    StaticJsonDocument<64> doc;
    String httpRequestData;

    doc["light_switch_1"] = array[r][0];
    doc["light_switch_2"] = array[r][1];
    doc["light_switch_3"] = array[r][2];

    digitalWrite(led_Pin1,array[r][0]);
    digitalWrite(led_Pin2,array[r][1]);   
    digitalWrite(led_Pin3,array[r][2]);

    serializeJson(doc, httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.PUT(httpRequestData);
    String http_response;

    // check reuslt of POST request. negative response code means server wasn't reached
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      Serial.print("HTTP Response from server: ");
      http_response = http.getString();
      Serial.println(http_response);

    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
     delay(2000);
   }
   
    // Free resources
    http.end();
   
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}