# BLYNK Meetup
## Prerequisites

[Supported Hardware](https://github.com/blynkkk/blynkkk.github.io/blob/master/SupportedHardware.md "Supported Hardware")

[Getting Started](https://www.blynk.cc/getting-started/ "Getting Started")

[Code Builder](http://examples.blynk.cc/ "Code Builder")

## IoT Hello World
```
// On ESP8266 Dev board
// Device --> Server --> Interface  | = Arduino uptime on Interface
// Device <-- Server <-- Interface  | = LED control on Device

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "your auth code"; // BLYNK App Project MeetupPart1
char ssid[] = "SSID"; 
char pass[] = "PASS";

BlynkTimer timer;

// FUNCTIONS
BLYNK_WRITE(V1) // Virtual Pin V1
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  if(pinValue == 1){
    digitalWrite(LED_BUILTIN, HIGH);
  }
   else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}
 
void upTimeV2() 
{
   Blynk.virtualWrite(V2, millis() / 1000);
}

// SETUP
void setup()
{
  Blynk.begin(auth, ssid, pass);
  pinMode(LED_BUILTIN, OUTPUT); 
                 // milis, function on timer  
  timer.setInterval(1000L, upTimeV2); 
}

// LOOP
void loop()
{
  Blynk.run();
  timer.run(); 
}
```
## Temperature Probe

![Part 2](images/MeetupPart2.PNG?raw=true "Part 2")

Add another LED - but to the interface

```
WidgetLED led1(V1);
bool ledStatus = false;
```





![IoT4Makers](images/BlynkMeetup2.png?raw=true "Meetup")
