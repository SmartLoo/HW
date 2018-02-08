//define pins numbers
const int trigPin = 7;
const int echoPin = 8; 
// defines variables
long duration;
int distance; 
String stringDistance;
String guid = "R52858b18-0bb7-45b0-836c-d4d5f335558e:";
String message;
String batteryLevel;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  digitalWrite(trigPin, LOW); // Clears the trigPin
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH); // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  
  distance= duration*0.034/2; // Calculating the distance
  stringDistance = String(distance);
  batteryLevel = ":75";
  message = guid += stringDistance += batteryLevel;
  digitalWrite(13, HIGH);
  Serial.println(message); // Prints the distance on the Serial Monitor
  //52858b18-0bb7-45b0-836c-d4d5f335558e
  //dac15423-20d7-4e97-80cd-0e46c98cd60e
  //Serial.write(distance);
  delay(5000);
  digitalWrite(13, LOW);
  delay(100);

}
