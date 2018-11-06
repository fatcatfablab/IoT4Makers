# BLYNK Meetup
IoT your project in 15 minutes for less that $15
## Prerequisites

[Arduino IDE](https://www.arduino.cc/en/Main/Software/ "Arduino IDE")

[BLYNK Getting Started](https://www.blynk.cc/getting-started/ "BLYNK Getting Started")

[Supported Hardware](https://github.com/blynkkk/blynkkk.github.io/blob/master/SupportedHardware.md "Supported Hardware")

[Sketch Generator](http://examples.blynk.cc/ "Sketch Generator")

## IoT Hello World
```
// On ESP8266 Dev board=(Device) Phone with Blynk App=(Interface) 
// Device --> Server --> Interface  | = Arduino uptime on Interface
// Device <-- Server <-- Interface  | = LED control on Device

// GLOBALS
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "your BLYNK Project auth code"; // BLYNK App Project MeetupPart1
char ssid[] = "WiFi SSID"; 
char pass[] = "WiFi PASS";

BlynkTimer timer;

// FUNCTIONS
BLYNK_WRITE(V1) // Device LED on Virtual Pin V1
{
  int pinValue = param.asInt(); // assigning incoming value from Interface pin V1 to a variable
  if(pinValue == 1){
    digitalWrite(LED_BUILTIN, HIGH);
  }
   else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void uptime()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}

// TIMING functions
void everySecond() 
{
  uptime();   
}

// SETUP
void setup()
{
  Blynk.begin(auth, ssid, pass);
  pinMode(LED_BUILTIN, OUTPUT); 
                 // milis, function on timer  
  timer.setInterval(1000L, everySecond); 
}

// LOOP
void loop()
{
  Blynk.run();
  timer.run(); 
}
```
Edit authentications above and upload to ESP8266 Dev Board 
Add Widget to Project in BLYNK App
Add Button Widget on V1
Add Value Display on V2

## Add LED and Terminal
Like Virtual hardware - on the BLYNK App

Add to GLOBALS
```
bool ledStatus = false;
// Arduino.ino file - version
char arduinoVersion[] = "BLYNKMeetupPart1.5";

// WIDGETS
WidgetLED led1(V3);
WidgetTerminal terminal(V4);
```
Add to FUNCTIONS
```
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

void debugPrint()
{
  if (debug) {
    terminal.print("ledStatus = ");
    terminal.println(ledStatus); 
  }
}
```
Add to Timing function everySecond()
```
blinkLedWidget();
debugPrint(); 
```
Add to SETUP
```
 led1.on();
 terminal.clear();
 terminal.println(F("Blynk v" BLYNK_VERSION ": Device started")); // Print Blynk Software version to the Terminal Widget on connection
 terminal.print("Arduino.ino file ");
 terminal.println(arduinoVersion);
 terminal.println(F("-------------"));
 terminal.println(F("Type 'debug on' or 'debug off' for debug Terminal prints"));
 terminal.flush();
```

[Final Code IoT Hello World](https://github.com/fatcatfablab/IoT4Makers/blob/master/BLYNKMeetup/BLYNKMeetupPart1.ino "Final Code IoT Hello World")

Upload changes to ESP8266

Add LED Widget to BLYNK app on V3

Add Terminal Widget to BLYNK app on V4 

## Temperature Probe Circuit
![Part 2](images/MeetupPart2.PNG?raw=true "Part 2")

## Add Temperature Probe and Alert 
Changes below but use [Final code for temperature probe](https://github.com/fatcatfablab/IoT4Makers/blob/master/BLYNKMeetup/BLYNKMeetupPart2.ino "Final Code Temperature Probe")

Add to GLOBALS
```
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
 const int tempPin = 14; // D5
```
Add to SETUP
```
sensors.begin(); // Temperature probe lib initialize
timer.setInterval(5000L, getTemp);
```

Add to FUNCTIONS
```
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

BLYNK_WRITE(V6)
{
  lowTempThreshold = param.asInt(); // assigning incoming value from pin V6 to a variable
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

```
Add to Timing function everySecond
```
  //blinkLedWidget(); 
  setLedWidget();
```
Add to SETUP
```
timer.setInterval(5000L, getTemp);
```
Add Slider Widget in Blynk App on V6

Add Temperature Alert Eventor Widget on V7

Alert Email Widget

[Final code for temperature probe](https://github.com/fatcatfablab/IoT4Makers/blob/master/BLYNKMeetup/BLYNKMeetupPart2.ino "Final Code Temperature Probe")

<!-- ![IoT4Makers](images/BlynkMeetup2.png?raw=true "Meetup") -->
