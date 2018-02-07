#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>  
#include <SoftwareSerial.h>

//helloooooo
const char* ssid     = "Sarah's iPhone";
const char* password = "aaaaaaaa";
const char* host = "smartloo.azurewebsites.net";

int IncomingByte = 0;
int target = 0;
int data = 0;
int code = 0;

//SoftwareSerial Serial(12,14);

void setup()
{
  Serial.begin(9600);
  delay(100);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  //if wifi is not connected
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
  while (!Serial.available());      // Wait for UART rx buffer to get entry
  int target = GetData();
  PostSensorLevel(target);
}

int GetData()
{
  if (Serial.available() > 0)
  {
    IncomingByte = Serial.read();
    return IncomingByte;
  }
}

void PostSensorLevel(String sensorId, int sVal, int bLevel)
{
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("connection failed");
  }
  
  // We now create a URL for the request
  String url = "/api/sensor_data/update";
  String sensorValue = String(sVal, DEC);
  String batteryLevel = String(bLevel, DEC);
  String req = "{\"sensorId\": \"" + sensorId + "\",\"sensorValue\": \"" + sensorValue + "\",\"batteryLevel\": \"" + batteryLevel + "\"}";
 

  HTTPClient http;
  http.begin("http://smartloo.azurewebsites.net/api/sensor");
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(req);
  String payload = http.getString();

  Serial.println(val);
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();

  Serial.println("POST REQUEST MADE.");  
}
