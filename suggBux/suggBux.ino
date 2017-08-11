#include "Adafruit_Thermal.h"

// assets below
#include "logokubik.h"
#include "positive.h"
#include "neutral.h"
#include "negative.h"
//

#include "SoftwareSerial.h"
#define TX_PIN 6 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 5 // Arduino receive   GREEN WIRE   labeled TX on printer
SoftwareSerial mySerial(RX_PIN, TX_PIN);  // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);      // Pass addr to printer constructor

const int bp1 = 2; //positive
const int bp2 = 3; //neutral
const int bp3 = 4; //negative

bool idleState = true;

static const unsigned long timeoutInterval = 1000 * 10; // rh seconds
static unsigned long lastRefreshTime = 0;


void setup() {
  // put your setup code here, to run once:
   pinMode(bp1, INPUT_PULLUP);
   pinMode(bp2, INPUT_PULLUP);
   pinMode(bp3, INPUT_PULLUP);
   
   Serial.begin(9600); //delete me

   pinMode(7, OUTPUT); digitalWrite(7, LOW);
   mySerial.begin(19200);  // Initialize SoftwareSerial
   printer.begin();        // Init printer (same regardless of serial type)
}

void loop() {
  int btn1V = digitalRead(bp1);
  int btn2V = digitalRead(bp2);
  int btn3V = digitalRead(bp3);
  
   if (btn1V == LOW){
      printerPrep(1);
   }
   if (btn2V == LOW){
      printerPrep(2);
   }
   if (btn3V == LOW){
      printerPrep(3);
   }

   timeCheck();
}

void printerPrep(int x){
    if (idleState == true){
      Serial.println("hit");
      lastRefreshTime = millis();
      idleState = false;
      printerStart(x);
    }
}

void timeCheck(){
      if ( (millis() - lastRefreshTime) >= timeoutInterval ) {
        if (idleState == false) {
          Serial.println("ready");
        }
        idleState = true;
      }
}

void printerStart(int x){
  Serial.println(x);
  printer.sleep();
  
  String chosen = "";
  switch (x) {
    case 1: //positive
      positivePrint();
      break;
    case 2: //neutral
      neutralPrint();
      break;
    case 3: //negative
      negativePrint();
    break;
  }
}

void brandingPrint(){
  printer.wake();
  printer.printBitmap(logokubik_width, logokubik_height, logokubik_data);
  printer.justify('C');
  printer.boldOn();
  printer.println(F("Primer Coworking de Barcelona"));
  printer.boldOff(); 
  printer.underlineOn();
  printer.println(F("http://www.kubikbcn.com/"));
  printer.underlineOff();
  printer.sleep();
}
void printerNewLine(){ //assumption of woke printer
  printer.doubleHeightOn();
  printer.println(F(" "));
  printer.doubleHeightOff();
}

void printerInputArea(){ //assumption of woke printer
  printer.doubleHeightOn();
  printer.println(F("Por que te sientes asi?"));
  printer.underlineOn();
  printer.println(F("                        "));
  printer.println(F("                        "));
  printer.println(F("                        "));
  printer.println(F("                        "));
  printer.underlineOff();
  printer.doubleHeightOff();
}

void positivePrint(){
  brandingPrint();
  Serial.println("positive");
  printer.wake();
  printerNewLine();
  printer.printBitmap(positive_width, positive_height, positive_data);
  printerInputArea();
  printerNewLine();
  printer.feed(2);
  printer.sleep();
}
void neutralPrint(){
  brandingPrint();
  Serial.println("neutral");
  printer.wake();
  printerNewLine();
  printer.printBitmap(neutral_width, neutral_height, neutral_data);
  printerInputArea();
  printerNewLine();
  printer.feed(2);
  printer.sleep();
}
void negativePrint(){
  brandingPrint();
  Serial.println("negative");
  printer.wake();
  printerNewLine();
  printer.printBitmap(negative_width, negative_height, negative_data);
  printerInputArea();
  printerNewLine();
  printer.feed(2);
  printer.sleep();
}
