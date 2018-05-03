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

#include <avr/sleep.h>
volatile int sleepCycleCounter = 0;
const int minutesToSleep = 5;
const int minSleepCycles = (minutesToSleep*60)/8;

const int enableXBee = 4;
const int enable5V = 6;
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;

const String DEVICE_GUID = "PI4614NJ";
String stringDistance;
String message;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

void setup() {
  for(int i = 0; i <= 20; i++){
    if(i != 8){
      pinMode(i, OUTPUT);
    }
  }

  WDTCSR = (24);//change enable and WDE - also resets
  WDTCSR = (33);//prescalers only - get rid of the WDE and WDCE bit
  WDTCSR |= (1<<6);//enable interrupt mode

  SMCR |= (1 << 2); //power down mode
  SMCR |= 1;//enable sleep
  
  Serial.begin(9600);
}

void loop() {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

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

  cm = pulse_width / 58.0;

  stringDistance = String((int)cm);
  message = DEVICE_GUID + ":" + stringDistance + "?";

  digitalWrite(enableXBee, HIGH);
  delay(500);
  if (cm < 200)
  {
      Serial.println(message);
  }
  delay(500);
  digitalWrite(enable5V, LOW);
  digitalWrite(enableXBee, LOW);

  for(int i = 0; i < minSleepCycles; i++){
    MCUCR |= (3 << 5); //set both BODS and BODSE at the same time
    MCUCR = (MCUCR & ~(1 << 5)) | (1 << 6); //then set the BODS bit and clear the BODSE bit at the same time
    __asm__  __volatile__("sleep");//in line assembler to go to sleep  
  }
}

ISR(WDT_vect)
{

}
