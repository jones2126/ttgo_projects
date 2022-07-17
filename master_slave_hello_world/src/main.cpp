/*********
  Sketch showing how to create a simple messaging client with the RH_RF69 class.
  
  Partial Credit: https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/

*********/


#include <Arduino.h>

//Libraries for LoRa
#include <SPI.h>
//#include <RH_RF69.h>  // http://www.airspayce.com/mikem/arduino/RadioHead/
#include <RadioHead.h>


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
  if (!Serial) {
      while(1) delay(1000);   // loop forever and don't continue
  }
  Serial.println("running slave_remote_control_Simple");
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
}
void startLoRA(){

//post message on OLED
  display.setCursor(0,10);
  display.clearDisplay();
  display.print("LoRa Initializing");
  display.display();
  
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);  
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  SPI.begin(RFM95_SCK, RFM95_MISO, RFM95_MOSI, RFM95_CS); //SPI LoRa pins 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(LORA_TxPower, false);

//post message on OLED
  display.setCursor(0,10);
  display.clearDisplay();
  display.print("LoRa Initializing OK!");
  display.display();
  
}