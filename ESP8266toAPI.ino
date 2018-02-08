#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>  
#include <SoftwareSerial.h>

const char* ssid     = "Sarah's iPhone";
const char* password = "aaaaaaaa";
const char* host = "smartloo.azurewebsites.net";

int IncomingByte = 0;
int target = 0;
int data = 0;
int code = 0;

void setup()
{
  Serial.begin(9600);
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
  while (!Serial.available());
  
  String data = Serial.readString();
  
  int delimId = data.indexOf(':');
  int delimValue = data.indexOf(':', delimId + 1);
  
  String sensorId = data.substring(0, delimId);
  String sensorValue = data.substring(delimId + 1, delimValue);
  String batteryLevel = data.substring(delimValue + 1);
  
  PostSensorLevel(sensorId, sensorValue, batteryLevel);
}

void PostSensorLevel(String sensorId, string sVal, string bLevel)
{
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("connection failed");
  }
  
  String url = "/api/sensor";
  
  //String sensorValue = String(sVal, DEC);
  //String batteryLevel = String(bLevel, DEC);
  
  String req = "{\"sensorId\": \"" + sensorId + "\",\"sensorValue\": \"" + sensorValue + "\",\"batteryLevel\": \"" + batteryLevel + "\"}";
 

  HTTPClient http;
  http.begin("http://smartloo.azurewebsites.net/api/sensor");
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(req);
  String payload = http.getString();

  Serial.println(httpCode);
  Serial.println(payload);
  http.end();

  Serial.println("POST REQUEST MADE.");  
}
