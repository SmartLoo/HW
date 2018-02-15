/*macro definition of water sensor and the buzzer*/
#define WATER_SENSOR 8
#define BUZZER 12
int trigger;
String StringTrigger;
String guid = "W52858b18-0bb7-45b0-836c-d4d5f335558e:";
String message;
String batteryLevel;

void setup()
{
    Serial.begin(9600);
    pins_init();
}
void loop()
{
    if(isExposedToWater()){
      soundAlarm();
    }
    else{
        StringTrigger=String(0);
        batteryLevel = ":75";
        digitalWrite(LED_BUILTIN, LOW);
        message = guid + StringTrigger + batteryLevel;
        Serial.println(message);
    }
      delay(41579);
      
}

void pins_init()
{
    pinMode(WATER_SENSOR, INPUT);
    //pinMode(BUZZER, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

/************************************************************************/
/*Function: When the sensor is exposed to the water, the buzzer sounds  */
/*          for 2 seconds.                                              */
/************************************************************************/
void soundAlarm()
{

        //digitalWrite(BUZZER, HIGH);.
        batteryLevel = ":75";
        digitalWrite(LED_BUILTIN, HIGH);
        trigger=1;
        StringTrigger = String(trigger);
        message = guid + StringTrigger + batteryLevel;
        Serial.println(message);

}

/************************************************************************/
/*Function: Determine whether the sensor is exposed to the water        */
/*Parameter:-void                                                       */
/*Return:   -boolean,if it is exposed to the water,it will return true. */
/************************************************************************/
boolean isExposedToWater()
{
    if(digitalRead(WATER_SENSOR) == LOW)
    return true;
    else return false;
}
