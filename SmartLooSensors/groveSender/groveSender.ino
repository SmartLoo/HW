/**
 * Hardware Connections:
 *  Arduino | Grove Water Sensor 
 *  -------------------
 *    5V    |   VCC      
 *    8     |   SIGNAL     
 *    GND   |   GND
 *  
 */

#include <avr/sleep.h>
volatile int sleepCycleCounter = 0;
const int minutesToSleep = 1;
const int minSleepCycles = (minutesToSleep*60)/8;

const int enableXBee = 4;
const int enable5V = 6;
const int WATER_SENSOR_SIGNAL_LINE = 8;

const String DEVICE_GUID = "LRV2PVDX";
String message;

void setup()
{
  for(int i = 0; i <= 20; i++){
    if(i != 8){
      pinMode(i, OUTPUT);
    }
  }
  pinMode(WATER_SENSOR_SIGNAL_LINE, INPUT);
  watchdogOn(); // Turn on the watch dog timer.
  Serial.begin(9600);
}

void loop()
{
  enterSleep();

  if (sleepCycleCounter > minSleepCycles) {
    digitalWrite(enable5V, HIGH);
    delay(100);
    if(digitalRead(WATER_SENSOR_SIGNAL_LINE) == LOW){
      message = DEVICE_GUID + ":1?";
    }
    else{
      message = DEVICE_GUID + ":0?";
    }
    digitalWrite(enableXBee, HIGH);
    delay(500);
    Serial.println(message);
    delay(500);
    digitalWrite(enable5V, LOW);
    digitalWrite(enableXBee, LOW);
    sleepCycleCounter = 0;
  } 
}

void enterSleep()   
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
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
