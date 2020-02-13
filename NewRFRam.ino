#include <SPI.h>
#include "Adafruit_FRAM_SPI.h"
#include <RH_RF95.h>

//Defines for LoRa board
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 430.65

//Defines for FRAM chip
#define FRAM_SCK 13
#define FRAM_MISO 12
#define FRAM_MOSI 11
#define FRAM_CS 8
#define FRAM_HOLD 7
RH_RF95 rf95(RFM95_CS, RFM95_INT);
Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CS);

void setup() {
  Serial.begin(9600);
  //Let's init the LoRa board first
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  if(rf95.init()) {
    Serial.println("Poggers");
  } else {
    Serial.println("NIGGER");
    for(;;);
  }
  if(rf95.setFrequency(RF95_FREQ)) {
    Serial.print("Frequency set to "); Serial.println(RF95_FREQ);
  } else {
    Serial.println("Frequency could not be set, bailing!");
    for(;;);
  }
  rf95.setTxPower(20, false);
  
  if(fram.begin()) {
    Serial.println("We out here");
    uint8_t stat = fram.getStatusRegister();
    Serial.print("FRAM STATUS: "); Serial.println(stat, HEX);
    uint8_t legg = fram.read8(0x01);
    Serial.print("Val at 0x01: "); Serial.println(legg, HEX);
    //Dump up to RAM address 10
    uint8_t value;
    for (uint16_t a = 0; a < 10; a++) {
      value = fram.read8(a);
      if ((a % 32) == 0) {
        Serial.print("\n 0x"); Serial.print(a, HEX); Serial.print(": ");
      }
      Serial.print("0x"); 
      if (value < 0x1) 
        Serial.print('0');
      Serial.print(value, HEX); Serial.print(" ");
    }
  } else {
    Serial.println("Fuck");
    for(;;);
  }
  randomSeed(analogRead(A0));
  pinMode(4, INPUT);
}

void loop() {
  //for(;;);
  fram.writeEnable(true);
  fram.write8(0x01, (uint8_t)random(0,255));
  fram.writeEnable(false);
  uint8_t reader = fram.read8(0x01);
  Serial.println(reader, HEX);
  //Next, I want to write random values to the RAM
  if(digitalRead(4) == HIGH) {
    for(uint16_t a = 0; a < 128; a++) {
      uint8_t value = (uint8_t)random(0, 255);
      fram.writeEnable(true);
      fram.write8(a, value);
      fram.writeEnable(false);
    }
    //Next clear all other bytes in RAM
    for(uint16_t a = 128; a < 8192; a++) {
      uint8_t value = 0;
      fram.writeEnable(true);
      fram.write8(a, value);
      fram.writeEnable(false);
    }
  }
  //Next transmit bytes up to first zero byte. Copy to TX buffer first
  uint8_t chegg[136];
  for(uint16_t a; a < 130; a++) {
    uint8_t chungus = fram.read8(a);
    chegg[a] = chungus;
  }
  //Set callsign to end of chegg
  chegg[127] = 'K'; chegg[128] = 'N'; chegg[129] = '4'; chegg[130] = 'G'; chegg[131] = 'P'; chegg[132] = 'O';
  chegg[133] = 0;
  //Next dump chegg
  for(int x = 0; x < sizeof(chegg); x++) {
    Serial.print(chegg[x], HEX); Serial.print(" ");
  }
  Serial.println();
  rf95.send(chegg, sizeof(chegg));
  for(;;);
}
