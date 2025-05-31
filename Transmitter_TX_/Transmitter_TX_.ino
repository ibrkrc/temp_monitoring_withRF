#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>


#define DHTPIN 2         
DHT dht(DHTPIN, DHTTYPE);


RF24 radio(9, 10);       
const byte address[] = "00001";  


struct datas {
  float tempature;
  float humidity;
};

void setup() {
  Serial.begin(9600);
  dht.begin();
  

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening(); 
}

void loop() {
  datas data;
  data.tempature = dht.readTemperature();
  data.humidity = dht.readHumidity();

  
  if (isnan(data.tempature) || isnan(data.humidity)) {
    Serial.println("Sensor is not work.");
    delay(2000);
    return;
  }

  
  radio.write(&data, sizeof(data));

 
  Serial.print("Transmitted -> Tempature: ");
  Serial.print(data.tempature);
  Serial.print(" °C, Humidity: ");
  Serial.print(data.humidity);
  Serial.println(" %");

  delay(2000);  
}
