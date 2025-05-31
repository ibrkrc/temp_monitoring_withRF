#include <SPI.h>
#include <Wire.h>
#include <RF24.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>


#define I2C_ADDRESS 0x3C
SSD1306AsciiWire oled;


RF24 radio(9, 10); // CE, CSN
const byte address[] = "00001";


struct Veriler {
  float sicaklik;
  float nem;
};

void setup() {
  Wire.begin();
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  oled.clear();

  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    Veriler veri;
    radio.read(&veri, sizeof(veri));

    oled.clear();
    oled.setCursor(0, 0);
    oled.print("Tempature: ");
    oled.print(veri.sicaklik, 1);
    oled.println(" C");

    oled.setCursor(0, 1);
    oled.print("Humidity: ");
    oled.print(veri.nem, 1);
    oled.print(" %");

    Serial.print("Tempature: ");
    Serial.print(veri.sicaklik);
    Serial.print(" C, Humidity: ");
    Serial.print(veri.nem);
    Serial.println(" %");
  }
}
