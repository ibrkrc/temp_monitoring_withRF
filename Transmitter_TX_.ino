#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>

// DHT tanımları
#define DHTPIN 2         // DHT11 DATA pini Arduino'nun D2 pinine bağlı
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// nRF24L01 tanımları
RF24 radio(9, 10);       // CE, CSN
const byte address[] = "00001";  // Aynı adres alıcıda da kullanılmalı

// Gönderilecek veri yapısı
struct Veriler {
  float sicaklik;
  float nem;
};

void setup() {
  Serial.begin(9600);
  dht.begin();
  

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening(); // Gönderici mod
}

void loop() {
  Veriler veri;
  veri.sicaklik = dht.readTemperature();
  veri.nem = dht.readHumidity();

  // Hatalı okuma kontrolü
  if (isnan(veri.sicaklik) || isnan(veri.nem)) {
    Serial.println("DHT11'dan veri okunamadi.");
    delay(2000);
    return;
  }

  // RF ile gönder
  radio.write(&veri, sizeof(veri));

  // Seri monitöre yaz
  Serial.print("Gonderildi -> Sicaklik: ");
  Serial.print(veri.sicaklik);
  Serial.print(" °C, Nem: ");
  Serial.print(veri.nem);
  Serial.println(" %");

  delay(2000);  // DHT11 için 2 saniyelik minimum bekleme süresi
}
