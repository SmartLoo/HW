//define pins numbers
const int trigPin = 7;
const int echoPin = 8; 
// defines variables
long duration;
int distance; 

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
  digitalWrite(13, HIGH);
  Serial.print("Distance: "); // Prints the distance on the Serial Monitor
  Serial.println(distance);
  delay(4000);
  digitalWrite(13, LOW);
  delay(100);

}
