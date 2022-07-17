/*********
  Sketch showing how to create a simple messaging client with the RH_RF69 class.
  
  Partial Credit: https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/

*********/


#include <Arduino.h>

//Libraries for LoRa
#include <SPI.h>
#include <RadioHead.h>
#include <RH_RF69.h>  // http://www.airspayce.com/mikem/arduino/RadioHead/


//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// functions below
void startSerial();
void startOLED();
void startLoRA();

// LoRa pins for TTGO Semtech SX1276 ref: https://microcontrollerslab.com/ttgo-lora32-sx1276-oled-board-pinout-getting-started-with-arduino-ide/
#define RFM_MISO 19
#define RFM_MOSI 27
#define RFM_SCK 5
#define RFM_CS 18
#define RFM_INT 26
#define RFM_RST 14
#define RFM_TxPower 5 // defaults to 17; 20 is highest
#define RFM_SyncWord 0xF1  // changes the sync word to match the receiver; ranges from 0-0xFF, default 0x34
#define RFM_FREQ 915.0     // 433.0 Asia; 866.0 Europe; 915.0 North America

//RH_RF95 rf95(RFM95_CS, RFM95_INT);    // Instance of the radio driver

RH_RF69 rf69(RFM_CS, RFM_INT);    // Instance of the radio driver
//RH_RF69 rf69(15, 16);

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//packet counter
int counter = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup() {
    startSerial();
    startOLED();
    startLoRA();  
}

void loop() {
   
  Serial.println("Sending to rf69_server");
  // Send a message to rf69_server
  uint8_t data[] = "Hello World!";
  rf69.send(data, sizeof(data));
  rf69.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf69.waitAvailableTimeout(500))
  { 
    // Should be a reply message for us now   
    if (rf69.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is rf69_server running?");
  }
  delay(5000);
}
void startSerial(){
  Serial.begin(115200);
  Serial.println("running slave_remote_control_Simple");  
  Serial.print("In startSerial");
  if (!Serial) {
      while(1) delay(1000);   // loop forever and don't continue
  }
  Serial.println(".... Exiting startSerial");
}
void startOLED(){
  //reset OLED display via software
  Serial.println("In startOLED");
 // Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    while(1) delay(1000);   // loop forever and don't continue
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER");
  Serial.println(".... Exiting startOLED");
}
void startLoRA(){
  Serial.println("In startLoRA");

  display.setCursor(0,10); //post message on OLED
  display.clearDisplay();
  display.print("LoRa Initializing");
  display.display();

  pinMode(RFM_RST, OUTPUT);
  digitalWrite(RFM_RST, HIGH);
  delay(100);  
  digitalWrite(RFM_RST, LOW);
  delay(10);
  digitalWrite(RFM_RST, HIGH);
  delay(10);
  SPI.begin(RFM_SCK, RFM_MISO, RFM_MOSI, RFM_CS); //SPI LoRa pins 
  while (!rf69.init()) {
    Serial.println("LoRa radio init failed");
    while(1) delay(1000);   // loop forever and don't continue
  }
  Serial.println("LoRa radio init OK!");

  if (!rf69.setFrequency(RFM_FREQ)) {
    Serial.println("setFrequency failed");
    while(1) delay(1000);   // loop forever and don't continue
  }
  Serial.print("... Freq set to: "); Serial.print(RFM_FREQ); Serial.print(") ... ");
  rf69.setTxPower(RFM_TxPower, false);

  display.setCursor(0,10);  //post message on OLED
  display.clearDisplay();
  display.print("LoRa Initializing OK!");
  display.display();
  Serial.print(".... Exiting startLoRA");
}