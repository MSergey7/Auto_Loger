//Программа должна будет сканировать температуру  и отправлять показания
//SMS при понижении температуры ниже чем +2 градуса
//и звонить при понижении ниже чем -2 градуса

//содержание СМС: 
//дата.время - температура //// "29.12.2016 15:54 = -2 Celcia"

#include <Wire.h>
#include <OneWire.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

//инициализируем переменные
OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary) - датчик температуры DS18B20

byte data[12];
byte addr[8];
float celsius;
  
void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("");
  Serial.println("-------------------");
}

void loop() {
//1. ищем датчик вызов
search_temperatur();

  
}

//1. ищем датчик функ
int search_temperatur(){
  
if ( !ds.search(addr)) {
//    Serial.println("No more addresses.");
    ds.reset_search();
    delay(250);
    return;
  }
 if (OneWire::crc8(addr, 7) != addr[7]) {
     Serial.println("CRC is not valid!");
      return 1 ;
  }
 else{
  Serial.println("CRC is valid!");
   return 0 ;
 }



}

//2. читаем температуру Функц
int read_temperatur(){


}
}

