# BLYNK Meetup
IoT your project in 15 minutes for less that $15
## Prerequisites

[Supported Hardware](https://github.com/blynkkk/blynkkk.github.io/blob/master/SupportedHardware.md "Supported Hardware")

[Getting Started](https://www.blynk.cc/getting-started/ "Getting Started")

[Code Builder](http://examples.blynk.cc/ "Code Builder")

## IoT Hello World
```
// On ESP8266 Dev board=(Device) Phone=(Interface) 
// Device --> Server --> Interface  | = Arduino uptime on Interface
// Device <-- Server <-- Interface  | = LED control on Device

// GLOBALS
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "your auth code"; // BLYNK App Project MeetupPart1
char ssid[] = "SSID"; 
char pass[] = "PASS";

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
## Add LED and Terminal
On the BLYNK App

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


```
Add to Timing function everySecond
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
Add LED Widget to BLYNK app on V3

Add Terminal Widget to BLYNK app on V4 

## Temperature Probe Circuit
![Part 2](images/MeetupPart2.PNG?raw=true "Part 2")

## Add Temperature Probe
Add to GLOBALS
```
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float Fahrenheit=0;
```
Add to SETUP
```
sensors.begin(); // Temperature probe lib initialize
```

Add to FUNCTIONS
```
void getTemp()
{
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Blynk.virtualWrite(V3, Fahrenheit); // Send temperature to Virtual Pin 3
}
```
Add to Timing function everySecond
```
getTemp();
```
Add Labled Value Display to BLYNK App on V3 - PUSH

## Setup Temperature Alert
Add to GLOBALS
```
int tempTrigger = 50;
```
Alert level Slider
Alert email


![IoT4Makers](images/BlynkMeetup2.png?raw=true "Meetup")
