#include <DHT.h>
#include <RF24.h>
#include <SPI.h>

#define DHTPIN 2

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

RF24 radio(7, 8); 

const byte address[6] = "node0";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  //const char text[] = "Hello World";
  //radio.write(&text, sizeof(text));
  //delay(1000);
  float temp = dht.readTemperature();
  delay(100);
  float hum = dht.readHumidity();
  delay(100);
  float sendData[] = {temp, hum};
  Serial.println(temp);
  Serial.println(hum);
  radio.write(&sendData, sizeof(sendData));
  radio.powerDown();
  delay(30000);
  radio.powerUp();
  delay(50);
}
