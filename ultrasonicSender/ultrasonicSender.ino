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
const int minutesToSleep = 1;
const int minSleepCycles = (minutesToSleep*60)/8;

// Pins
const int enable5V = 6;
const int enableXBee = 4;
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;
const String DEVICE_GUID = "Pdac15423-20d7-4e97-80cd-0e46c98cd80e";
String stringDistance;
String batteryLevel;
String message;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

void setup() {

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(enable5V, OUTPUT);
  pinMode(enableXBee, OUTPUT);
  //digitalWrite(TRIG_PIN, LOW);
  for(int i = 0; i <= 20; i++){
    if(i != 8){
      digitalWrite(i, LOW);
    }
  }

  watchdogOn(); // Turn on the watch dog timer.

  // Disable the ADC by setting the ADEN bit (bit 7) to zero.
  ADCSRA = ADCSRA & B01111111;

  // Disable the analog comparator by setting the ACD bit
  // (bit 7) to one.
  ACSR = B10000000;

  // Disable digital input buffers on all analog input pins
  // by setting bits 0-5 to one.
  DIDR0 = DIDR0 | B00111111;

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}

void loop() {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  enterSleep(); // ATmega328 goes to sleep for about 8 seconds
  // and continues to execute code when it wakes up

  if (sleepCycleCounter >= minSleepCycles) {
    digitalWrite(enable5V, HIGH);
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

    // Calculate distance in centimeters and inches. The constants
    // are found in the datasheet, and calculated from the assumed speed 
    //of sound in air at sea level (~340 m/s).
    cm = pulse_width / 58.0;

    stringDistance = String((int)cm);
    batteryLevel = ":75";
    message = DEVICE_GUID + stringDistance + batteryLevel;

    digitalWrite(enableXBee, HIGH);

    if (cm < 200)
    {
        Serial.println(message); // Prints the distance on the Serial Monitor
    }
    digitalWrite(enable5V, LOW);
    digitalWrite(enableXBee, LOW);
  }
}

void enterSleep()   
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  // After waking from watchdog interrupt the code continues
  // to execute from this point.

  sleep_disable();                   
}

void watchdogOn() {
  
  // Clear the reset flag, the WDRF bit (bit 3) of MCUSR.
  MCUSR = MCUSR & B11110111;
  
  // Set the WDCE bit (bit 4) and the WDE bit (bit 3) 
  // of WDTCSR. The WDCE bit must be set in order to 
  // change WDE or the watchdog prescalers. Setting the 
  // WDCE bit will allow updtaes to the prescalers and 
  // WDE for 4 clock cycles then it will be reset by 
  // hardware.
  WDTCSR = WDTCSR | B00011000; 

  // Set the watchdog timeout prescaler value to 1024 K 
  // which will yeild a time-out interval of about 8.0 s.
  WDTCSR = B00100001;

  // Enable the watchdog timer interupt.
  WDTCSR = WDTCSR | B01000000;
  MCUSR = MCUSR & B11110111;

}

ISR(WDT_vect)
{
  sleepCycleCounter ++;
}
