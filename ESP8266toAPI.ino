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

//SoftwareSerial Serial(12,14);

void setup()
{
  //Serial.begin(111111); 
  //Serial.begin(115200); //for ESP  
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
  //Serial.print("Netmask: ");
  //Serial.println(WiFi.subnetMask());
  //Serial.print("Gateway: ");
  //Serial.println(WiFi.gatewayIP());

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

void GetSensorLevel()
{
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
  }
  // We now create a URL for the request
  String url = "/api/sensor";

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);

  // Read all the lines of the reply from server and print them to Serial
  if (client.available())
  {
    String line = client.readStringUntil('"');
    data = client.readStringUntil('"').toInt();
    Serial.write((unsigned char)data);
    Serial.flush();
    Serial.println((unsigned char)data);
  }
}

void PostSensorLevel(int t)
{
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("connection failed");
  }
  
  // We now create a URL for the request
  String url = "/api/sensor_data/update";
  String val = String(t, DEC);
  String req = "{\"building\": \"PHO\",\"location\": \"1FLOORW\",\"sensorId\": \"302\",\"value\":" + val + ",\"battery\": 10}";
 

  HTTPClient http;
  http.begin("http://smartloo.azurewebsites.net/api/sensor_data/update");
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(req);
  String payload = http.getString();

  Serial.println(val);
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();

/*
  // This will send the request to the server
  client.print("POST /api/sensor_data/update HTTP/1.1");
  client.print("Host: smartloo.azurewebsites.net");
  client.println("Content-Type: application/json"); 
  client.println("Accept: application/json");
  client.print("Content-Length: ");
  client.println(req.length());
  client.println();
  client.println(req);
*/

  Serial.println("POST REQUEST MADE.");  
}
