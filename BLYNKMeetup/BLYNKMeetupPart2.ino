/*

  Temp Probe on Blynk App and ESP8266 Dev board  
   
   Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
*/

/*                      
 *                       Board NodeMCU 1.0 ESP-12E
 *                         
 *                         ESP 8266 12E   Pin       
 *                       --A0     D0--GPIO16-- |X Needed for deep sleep
 *                       --GND    D1--GPIO5--  | -- Future Version - 470ohm Resistor -- Neopixel
 *                       --VV     D2--GPIO4----|
 *                       --SD3    D3--GPIO0----| -- Led
 *                       --SD2    D4--GPIO2----|XX BuiltIn LED pin 2 and Bootloader 
 *                       --SD1    3V3--3.3V----|V+ 
 *                       --CMD    GND--GPIO----|
 *                       --SD0    D5--GPIO14---| -- DS18B20 Temp Probe + 4.7k pullup resistor
 *                       --CLK    D6--GPIO12---| -- 
 *                       --GND    D7--GPIO13---| -- 
 *                       --3V3    D8--GPIO15---|xx ? Conflicts with serial output
 *                       --EN     RX--
 *                       --RST    TX--
 *                 Ground--GND    GND----------|
 *                 Vin 5v--Vin    3.3V-- 
 *  
*/

/* 
 * Blynk App > Device - Blynk Example - Get Data 
 * BLYNK_WRITE(V1){ //Called every time Widget on V1 in Blynk app writes values to V1
 *  int pinValue = param.asInt(); // assigning incoming value from Interface pin V1 to a variable
 *  }
 *  
 * Device > Blynk App - Blynk Example - Push Data
 *  Blynk.virtualWrite(V7, value); //Trigger for Eventor Notifications
 *  
 * Blynk App Request < From Device - Blynk Example - Push Data On Request
 * BLYNK_READ(V8){
 *  Blynk.virtualWrite(PIN_UPTIME, millis() / 1000);
 *{
 *
 */

/* Comment this out to disable prints and save space */

#define BLYNK_PRINT Serial

// GLOBALS
// Libraries
// ESP32 
// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <BlynkSimpleEsp32.h>

// ESP8266 
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Temp Probe 
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 float Celcius=0;
 float Fahrenheit=0;
 int lowTempThreshold = 50; // Default thresholds
 int highTempThreshold = 90;
 bool tempAlert = false;
 

// Objects
BlynkTimer timer;

WidgetLED led1(V3);
bool ledStatus = false;

WidgetTerminal terminal(V4);
bool debug = false;

char arduinoVersion[] = "BLYNKMeetupPart2-V3";

// AUTHORIZATIONS
// Blynk Token
char auth[] = "blynk app auth code"; 

// WiFi
// Set password to "" for open networks.

//char ssid[] = "wifi SSID"; 
//char pass[] = "wifi Pass";

//Timing when offline !!Switch to use the Blynk Timer when offline??
int lastConnectionAttempt = millis();
int connectionDelay = 5000; // try to reconnect every 5 seconds

//Timer5Sec
int lastRun5 = millis();

//Time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

//Physical Pins
const int ledPin = 0; // D3
const int tempPin = 14; // D5
//const int buttonPin = 13; // D7


/* 
 * Virtual Pins - Feature
 * V1 - Button > ledPin - Hardware LED
 * V2 - Text Widget < Arduino UpTime
 * V3 - LED Widget < Temp Treshold
 * V4 - Terminal Widget <> Send and Receive 
 * V5 - DS18B20 Temp Probe
 * V6 - Slider
 * V7 - tempAlert > Eventor
 */


// FUNCTIONS 

BLYNK_WRITE(V1) // Device LED on Virtual Pin V1
{
  int pinValue = param.asInt(); // assigning incoming value from Interface pin V1 to a variable
  if(pinValue == 1){
    //digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(ledPin, HIGH);
  }
   else{
    //digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(ledPin, LOW);
  }
}

BLYNK_WRITE(V6)
{
  lowTempThreshold = param.asInt(); // assigning incoming value from pin V6 to a variable
}

void blinkLedWidget()
{
  if (ledStatus) {
    led1.setColor("#D3435C"); //Red
    ledStatus = false;
  } else {
    led1.setColor("#23C48E"); //Green
    ledStatus = true;
  }
}

void setLedWidget()
{
  if(lowTempThreshold > Fahrenheit)
  {
    tempAlert = true;
    led1.setColor("#D3435C"); //Red
    Blynk.virtualWrite(V7, 1); //Trigger for Eventor Notifications
    //Serial.println("Red");
  }
  if(lowTempThreshold < Fahrenheit)
  {
    tempAlert = false;
    led1.setColor("#23C48E"); //Green
    Blynk.virtualWrite(V7, 0);
    //Serial.println("Green");
  }

}



void debugPrint()
{
  if (debug) {
    //terminal.print("ledStatus = ");
    //terminal.println(ledStatus); 
    terminal.print("lowTempThreshold = ");
    terminal.println(lowTempThreshold); 
  }
}
 
void uptime()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}

BLYNK_WRITE(V4) // Terminal Widget on V4
{
    if (String("debug on") == param.asStr()) {
    debug = true;
    terminal.println("ON") ;
    } 
    
    if (String("debug off") == param.asStr()){
    debug = false;
    terminal.println("OFF") ;
    
    // Send it back
    terminal.print("Debug is ");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }

  // Ensure everything is sent
  terminal.flush();
}

// TIMING functions
void everySecond() 
{
  uptime();
  //blinkLedWidget(); 
  setLedWidget();
  debugPrint();  
}


void getTemp()
{
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  //Serial.print(" C  ");
  //Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit); 
  Blynk.virtualWrite(V5, Fahrenheit); // Send temp to Virtual Pin 5
}


// SETUP 
void setup()
{
Serial.begin(9600);

sensors.begin(); // Temperature probe lib initialize

Blynk.begin(auth, ssid, pass);

pinMode(ledPin, OUTPUT);
//pinMode(LED_BUILTIN, OUTPUT); 

  // Call Function on timer
  //               delay - function
 timer.setInterval(1000L, everySecond); 
 timer.setInterval(5000L, getTemp);

 led1.on();

  terminal.clear();
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started")); // Print Blynk Software version to the Terminal Widget on connection
  terminal.print("Arduino.ino file ");
  terminal.println(arduinoVersion);
  terminal.println(F("-------------"));
  terminal.println(F("Type 'debug on' or 'debug off' for debug prints"));
  terminal.flush();
}

// LOOP
void loop()
{
  // check WiFi connection:
  if (WiFi.status() != WL_CONNECTED)
  {
    
// OFFLINE CODE
    Serial.println(" Offline ...");
    //!! So here we can use the blynk timer call?
    // check delay:
    if (millis() - lastConnectionAttempt >= connectionDelay)
    {
      lastConnectionAttempt = millis();
      Serial.println(" Connecting ...");
      // attempt to connect to Wifi network:
      if (pass && strlen(pass))
      {
        WiFi.begin((char*)ssid, (char*)pass);
      }
      else
      {
        WiFi.begin((char*)ssid); // For open networks I guess.
      }
    }
  }
  else
  { 
 
// BLYNK ONLINE CODE 

    Blynk.run();
    timer.run(); // BlynkTimer

  }
}

