// On ESP8266 Dev board=(Device) Phone=(Interface) 
// Device --> Server --> Interface  | = Arduino uptime on Interface
// Device <-- Server <-- Interface  | = LED control on Device

// GLOBALS
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "Blynk App Auth code"; 

char arduinoVersion[] = "BLYNKMeetupPart1-V5";

char ssid[] = "WiFi SSID";
char pass[] = "WiFi Pass";

WidgetLED led1(V3);
bool ledStatus = false;

WidgetTerminal terminal(V4);
bool debug = false;

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

void debugPrint()
{
  if (debug) {
    terminal.print("ledStatus = ");
    terminal.println(ledStatus); 
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
  blinkLedWidget(); 
  debugPrint();  
}

// SETUP
void setup()
{
  Blynk.begin(auth, ssid, pass);
  pinMode(LED_BUILTIN, OUTPUT); 
                 // milis, function on timer  
  timer.setInterval(1000L, everySecond); 

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
  Blynk.run();
  timer.run(); 
}
