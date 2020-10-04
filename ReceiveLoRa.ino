

#include "SPI.h"
#include "LoRa.h"
#include "SSD1306.h"

// WIFI_LoRa_32 ports

#define SCK 5 // GPIO5 - SX1278's SCK
#define MISO 19 // GPIO19 - SX1278's MISO
#define MOSI 27 // GPIO27 - SX1278's MOSI
#define SS 18 // GPIO18 - SX1278's CS
#define RST 14 // GPIO14 - SX1278's RESET
#define DI0 26 // GPIO26 - SX1278's IRQ (interrupt request)
#define BAND 915E6 //915Mhz for USA

SSD1306 display (0x3c, 4, 15);
bool last = HIGH;
String rssi = "RSSI   ";
String packSize = "--";
String packet ;

void loraData(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 , 15 , "Received "+ packSize + " bytes");
  display.drawStringMaxWidth(0 , 35 , 128, packet);
  display.drawString(0, 0, rssi);  
  display.display();
  digitalWrite(2, last);
  last = !last;
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  Serial.println(packet);
  loraData();
}

void setup() {

  pinMode (16, OUTPUT);
  pinMode (2, OUTPUT);
  
  digitalWrite (16, LOW); // set GPIO16 low to reset OLED
  delay (50);
  digitalWrite (16, HIGH); // while OLED is running, GPIO16 must go high
  
  Serial.begin(115200);
  while (!Serial); //if just the the basic function, must connect to a computer
  
  delay(1000);
  
  SPI.begin (SCK, MISO, MOSI, SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  display.init ();
  display.flipScreenVertically ();
  display.setFont (ArialMT_Plain_10);
   display.clear ();
  display.setTextAlignment (TEXT_ALIGN_LEFT);
  display.clear ();
  display.drawString(30,20, "LoRa Receiver");
  Serial.println("LoRa Receiver");
  display.display ();
  delay (2500);
}


void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize); 
    }
 }

    
