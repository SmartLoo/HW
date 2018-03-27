/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "BU Guest (unencrypted)";
const char* password = "";

//#include <SoftwareSerial.h>
const char* host = "smartloo.azurewebsites.net";

int IncomingByte = 0;
int target = 0;
int data = 0;
int code = 0;

HardwareSerial Serial1(2);

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(100);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop()
{
  if (Serial1.available() >= 38)
  {
      String data = Serial1.readString();
      
      Serial.println("XBee Raw Data: " + data);
      int delimId = data.indexOf(':');
      int delimValue = data.indexOf(':', delimId + 1);
      
      String sensorId = data.substring(0, delimId);
      String sensorValue = data.substring(delimId + 1, delimValue);
      String batteryLevel = data.substring(delimValue + 1);

      Serial.println(sensorId);
      Serial.println(sensorValue);
      Serial.println(batteryLevel);

      if (sensorId.length() == 37){
          PostSensorLevel(sensorId, sensorValue, batteryLevel);
      }
  }
  delay(1000);
}

void PostSensorLevel(String sensorId, String sensorValue, String batteryLevel)
{
  WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("Connection failed.");
  }
  
  String url = "/api/sensor";
  String req = "{\"sensorId\": \"" + sensorId + "\",\"sensorValue\": " + sensorValue + ",\"batteryLevel\": " + batteryLevel + "}";
 
  HTTPClient http;
  http.begin("http://smartloo.azurewebsites.net/api/sensor");
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(req);
  String payload = http.getString();

  Serial.println(httpCode);
  http.end();
}
