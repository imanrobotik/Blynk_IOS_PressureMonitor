
//Merged codebases of IOTAPPstory and Blynk with my personal sketch pressure sensor to Ananlog input for collecting data
//notes: at this point the blyk wifi creds supercedd IOTAPP story creds, so you should setup the otappstory wifi the same router on setup
//original codes are IASBlink V1.0.2 from iotappstory and Blynk simple timer for ESP8266 both from their example libraries.
//todo:
//it would be nice to be ablt ot put the blynk token in on wifimanager setup web page and test the iotapp buttons


//IAS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  This is an initial sketch to be used as a "blueprint" to create apps which can be used with IOTappstory.com infrastructure
  Your code can be filled wherever it is marked.

  Copyright (c) [2017] [Andreas Spiess]

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  IASBlink V1.0.2
*/
#define COMPDATE __DATE__ __TIME__
#define MODEBUTTON 0                                        // Button pin on the esp for selecting modes. D3 for the Wemos!
#include <IOTAppStory.h>                                    // IotAppStory.com library
IOTAppStory IAS(COMPDATE, MODEBUTTON);                      // Initialize IOTAppStory
//IAS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Blynk///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  NOTE:
  BlynkTimer provides SimpleTimer functionality:
    http://playground.arduino.cc/Code/SimpleTimer

  App project setup:
    Value Display widget attached to Virtual Pin V5
 *************************************************************/
#define BLYNK_PRINT Serial // Defines the object that is used for printing
#define BLYNK_DEBUG        // Optional, this enables more detailed prints
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4a35cc17934c4dbe933bf0871e8021db";//change this to your Blynk token

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Grace 3G";//blynk usues this (not IOTAPPSTORY)
char pass[] = "aV34Th0L";

//My Sketch code////////////////////////////////////////////////////////////////////////////////
const int analog_ip = A0; //Naming analog input pin
int inputVal  = 0;        //Variable to store analog input values
float bar = 0;
//My Sketch code////////////////////////////////////////////////////////////////////////////////

BlynkTimer timer;

// .
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
//My Sketch code////////////////////////////////////////////////////////////////////////////////
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  inputVal = analogRead (analog_ip); // Analog Values 0 to 1023
  Serial.println (inputVal);
  bar = (inputVal*0.0075-0.5)*1.724;//calibrated figure for my sensor 
 // tests and calcs are done here
  Blynk.virtualWrite(V5, bar);//sends bar to blynk virtual pin 5 wich is tranmitted via restful api to my android app monitoring pin v5 values (pressure)
  //low pressure trigger
  if (bar  < 1) { 
  Serial.print("Low Pressure!!!!!");
  Blynk.notify("Cheviot Water Pressure is LOW!!");
  Blynk.email("patrickscott76@hotmail.com", "Blynk Alert", "Cheviot Water Pressure is LOW!!");
}
}
//My Sketch code////////////////////////////////////////////////////////////////////////////////

//Blynk///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










void setup()
{
//IAS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 IAS.preSetDeviceName("iasblink");                         // preset deviceName this is also your MDNS responder: http://iasblink.local
//IAS.addField(LEDpin, "ledpin", 2, 'P');                   // These fields are added to the config wifimanager and saved to eeprom. Updated values are returned to the original variable.
//IAS.addField(blinkTime, "Blinktime(mS)", 5, 'N');         // reference to org variable | field label value | max char return
// You can configure callback functions that can give feedback to the app user about the current state of the application.
// In this example we use serial print to demonstrate the call backs. But you could use leds etc.
 IAS.onModeButtonShortPress([]() {
    Serial.println(F(" If mode button is released, I will enter in firmware update mode."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
  });
  IAS.onModeButtonLongPress([]() {
    Serial.println(F(" If mode button is released, I will enter in configuration mode."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
  });
  IAS.begin('P');                                           // Optional parameter: What to do with EEPROM on First boot of the app? 'F' Fully erase | 'P' Partial erase(default) | 'L' Leave intact
  IAS.setCallHome(true);                                    // Set to true to enable calling home frequently (disabled by default)
  IAS.setCallHomeInterval(70);                              // Call home interval in seconds, use 60s only for development. Please change it to at least 2 hours in production
//IAS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
//Blynk///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Debug console
  Serial.begin(115200);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);//use this port8080 with tetthered 3g hotspot?
  //Blynk.begin(auth, ssid, pass, IPAddress(188,166,206,43), 80);
  // Setup a function to be called every n seconds
  timer.setInterval(30000L, myTimerEvent);
//Blynk///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void loop()
{
//IAS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   IAS.loop();                                // this routine handles the calling home on the configured itnerval as well as reaction of the Flash button. If short press: update of skethc, long press: Configuration
//IAS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Blynk///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
//Blynk///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}



