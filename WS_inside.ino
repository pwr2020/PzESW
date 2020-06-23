#include <RF24.h>
#include <Wire.h>
#include <DS3231.h>
#include <SPI.h>
#include <DHT.h>
#include "LCD.h"
#include "DEV_Config.h"

#define DHTPIN 2

#define DHTTYPE DHT22

RTClib RTC;

DS3231 Clock;

RF24 radio(5,6);
const byte address[6] = "node0";

DHT dht(DHTPIN, DHTTYPE);

float dataR[2] = {0,0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_DC, OUTPUT);

  dht.begin();

  Wire.begin();

  radio.begin();
  delay(100);
  radio.openReadingPipe(0, address);
  delay(100);
  radio.setPALevel(RF24_PA_MAX);
  delay(100);
  radio.startListening();
  delay(100);

  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
  delay(100);
  
  LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;
  LCD.LCD_Init(Lcd_ScanDir);
  delay(100);
  
  //Serial.println(F("Start"));
  LCD.LCD_DisplayString(50, 50, "Starting", &Font16, WHITE, BLACK);
}

void loop() {
   //put your main code here, to run repeatedly:
   
  char humInside[30] = "Wilg.: ";
  char tempInside[22] = "Temp.: ";
  char humOutside[30] = "Wilg.: ";
  char tempOutside[22] = "Temp.: ";
  char tempv[5];
  char humv[5];
  char tempvOut[5];
  char humvOut[5];
  char printTime[8];
  float temp;
  float hum;
  int ihour;
  int iminute;
  DateTime now;
  
  if (radio.available()) 
  {
    radio.read(dataR, sizeof(dataR));
    delay(100);
    //Serial.println(dataR[0]);
    //Serial.println(dataR[1]);
  }
  now = RTC.now();
 
  temp = dht.readTemperature();
  delay(100);
  hum = dht.readHumidity();
  delay(100);
  
  ihour = now.hour();
  iminute = now.minute();
    
  //Serial.print(F("Temp: "));
  //Serial.println(printTime);
 
  dtostrf(temp, 4, 2, tempv);
  strncat(tempInside, tempv, 5);
  strncat(tempInside, " C", 3);
  
  dtostrf(hum, 4, 2, humv);
  strncat(humInside, humv, 5);
  strncat(humInside, "%", 2);

  dtostrf(dataR[0], 4, 2, tempvOut);
  strncat(tempOutside, tempvOut, 5);
  strncat(tempOutside, " C", 3);
  
  dtostrf(dataR[1], 4, 2, humvOut);
  strncat(humOutside, humvOut, 5);
  strncat(humOutside, "%", 2);
  
  snprintf(printTime, 6, "%02d:%02d", ihour, iminute);
  
  LCD.LCD_Clear(WHITE);
  LCD.LCD_DisplayString(50, 5, "Wewn.", &Font16, WHITE, BLACK);
  LCD.LCD_DisplayString(5, 25, tempInside, &Font12, WHITE, BLACK);
  LCD.LCD_DrawCircle(93, 27, 2, BLACK, 0, 1);
  LCD.LCD_DisplayString(5, 35, humInside, &Font12, WHITE, BLACK);
  
  LCD.LCD_DisplayString(50, 50, printTime, &Font12, WHITE, BLACK);

  LCD.LCD_DisplayString(50, 70, "Zewn.", &Font16, WHITE, BLACK);
  LCD.LCD_DisplayString(5, 90, tempOutside, &Font12, WHITE, BLACK);
  LCD.LCD_DrawCircle(93, 92, 2, BLACK, 0, 1);
  LCD.LCD_DisplayString(5, 100, humOutside, &Font12, WHITE, BLACK);
  
  delay(10000);
}
