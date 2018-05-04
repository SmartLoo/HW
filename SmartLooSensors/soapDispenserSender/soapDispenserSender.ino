/**
 * Hardware Connections:
 *  Arduino | HC-SR04 
 *  -------------------
 *    5V    |   VCC     
 *    7     |   Trig     
 *    8     |   Echo     
 *    GND   |   GND
 *  
 */


const int enableXBee = 4;
const int enable5V = 6;
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;

bool depressed = false;
int depressedCounter = 0;
unsigned long initialTime;
unsigned long currentTime;
const String DEVICE_GUID = "SI4624NJ";
String message;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

void setup() {
  for(int i = 0; i <= 20; i++){
    if(i != 8){
      pinMode(i, OUTPUT);
    }
  }
  
  Serial.begin(9600);
}

void loop() {
  initialTime = millis();

  do{
    currentTime = millis();
    unsigned long t1;
    unsigned long t2;
    unsigned long pulse_width;
    float measuredDistance;
  
    digitalWrite(enable5V, HIGH);
    delay(100);
    // Hold the trigger pin high for at least 10 us
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
  
    // Wait for pulse on echo pin
    while ( digitalRead(ECHO_PIN) == 0 );
  
    // Measure how long the echo pin was held high (pulse width)
    // Note: the micros() counter will overflow after ~70 min
    t1 = micros();
    while ( digitalRead(ECHO_PIN) == 1);
    t2 = micros();
    pulse_width = t2 - t1;
  
    measuredDistance = pulse_width / 58.0;

    if(measuredDistance < 8.25 && depressed == false){
      depressed = true;
    }
    else if(measuredDistance > 8.25 && depressed == true){
      depressedCounter++;
    }
    delay(500);
  } while((currentTime - initialTime) < 30000);
    
  message = DEVICE_GUID + ":" + String((int)depressedCounter) + "?";

  digitalWrite(enableXBee, HIGH);
  delay(500);
  Serial.println(message);
  delay(500);
  digitalWrite(enable5V, LOW);
  digitalWrite(enableXBee, LOW);
  depressedCounter = 0;
}




