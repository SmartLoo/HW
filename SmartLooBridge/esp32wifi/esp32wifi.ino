
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
String bridgeId = "BR234567";

HardwareSerial XBee(2);

void setup()
{
  Serial.begin(9600);
  XBee.begin(9600, SERIAL_8N1, 16, 17);
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
  if (XBee.available() >= 11) //changed from 38 to 0
  {
      String data = XBee.readStringUntil('?');
      Serial.println(data);

      int delimId = data.indexOf(':');
      int delimValue = data.indexOf(':', delimId + 1);
      
      String sensorId = data.substring(0, delimId);
      sensorId.trim();
      bridgeId.trim();
      String sensorValue = data.substring(delimId + 1, delimValue);
      Serial.println("----------");
      Serial.println(sensorId);
      Serial.println(bridgeId);
      Serial.println(sensorValue);
      //Serial.println(sensorId.length());
      //Serial.println(bridgeId.length());
      Serial.println("----------");

      if (sensorValue.toInt() >= 0 && sensorValue.toInt() <= 200){
          Serial.println("POST START");
          PostSensorLevel(sensorId, bridgeId, sensorValue);
      }
      else
      {
        Serial.println("POST FAIL");
      }
  }
  delay(1000);
}

void PostSensorLevel(String sensorId, String bridgeId, String sensorValue)
{
  WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("Connection failed.");
  }
  
  String url = "/api/sensor";
  String req = "{\"sensorId\": \"" + sensorId + "\",\"bridgeId\": " + "\"" + bridgeId + "\"" + ",\"sensorValue\": " + sensorValue + "}";
  Serial.println(req);
  HTTPClient http;
  http.begin("http://smartloo.azurewebsites.net/api/sensor");
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(req);
  String payload = http.getString();

  Serial.println(httpCode);
  http.end();
}
