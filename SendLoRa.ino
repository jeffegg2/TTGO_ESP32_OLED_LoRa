
#include "SPI.h"
#include "LoRa.h"
#include "SSD1306.h"

#define SCK 5 // GPIO5 - SX1278's SCK
#define MISO 19 // GPIO19 - SX1278's MISO
#define MOSI 27 // GPIO27 - SX1278's MOSI
#define SS 18 // GPIO18 - SX1278's CS
#define RST 14 // GPIO14 - SX1278's RESET
#define DI0 26 // GPIO26 - SX1278's IRQ (interrupt request)
#define BAND 915E6 //915Mhz for USA

unsigned int counter = 0;

SSD1306 display (0x3c, 4, 15);
bool last = LOW;
String rssi = "RSSI -";
String packSize = "-";
String packet;


void setup () {
  pinMode (16, OUTPUT);
  pinMode (2, OUTPUT);
  
  digitalWrite (16, LOW); // set GPIO16 low to reset OLED
  delay (50);
  digitalWrite (16, HIGH); // while OLED is running, GPIO16 must go high
  
  Serial.begin (115200);
  while (! Serial);
  Serial.println ();
  Serial.println ("LoRa Sender Test");
  
  SPI.begin (SCK, MISO, MOSI, SS);
  LoRa.setPins (SS, RST, DI0);
  if (! LoRa.begin (BAND)) {
    Serial.println ("Starting LoRa failed!");
    while (1);
  }
  Serial.println ("init ok");
  display.init ();
  display.flipScreenVertically ();
  display.setFont (ArialMT_Plain_10);
  delay (1500);
}

void loop () {
  display.clear ();
  display.setTextAlignment (TEXT_ALIGN_LEFT);
  display.setFont (ArialMT_Plain_10);
  display.drawString (0, 0, "Sending packet:");
  display.drawString (90, 0, String (counter));
  display.display ();

  // send packet
  LoRa.beginPacket ();
  LoRa.print ("Hello World!! ");
  LoRa.print (counter);
  LoRa.endPacket ();

  counter ++;
  digitalWrite (2, last); // toggle the LED.
  delay (2000); // wait for two seconds
  last = !last;
  }
