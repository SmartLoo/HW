#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>  
#include <SoftwareSerial.h>

const char* ssid     = "iPhone";
const char* password = "smartwifi";
const char* host = "smartloo.azurewebsites.net";

int IncomingByte = 0;
int target = 0;
int data = 0;
int code = 0;

SoftwareSerial XBee(4,5);

void setup()
{
  Serial.begin(9600);
  XBee.begin(9600);
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
  if (XBee.available() >= 38)
  {
      String data = XBee.readString();
      
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
