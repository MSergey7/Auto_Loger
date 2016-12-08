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

byte i;
byte data[12];
byte addr[8];
float celsius;
double temperature;  // измеренная температура

  
void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("");
  Serial.println("-------начало работы ----------");
}

void loop() {
//1. ищем датчик вызов
search_temperatur();

//2. читаем температуру Функц
read_temperatur();
  
}

//1. ищем датчик функ
int search_temperatur(){
  
if ( !ds.search(addr)) {
//    Serial.println("No more addresses.");
    ds.reset_search();
    delay(250);
  // снять все ремарки если нужно поменять разрешение
    ds.reset(); // сброс шины
    ds.select(addr); //выставить адрес
    ds.write(0x4E); // разрешение записать конфиг
    ds.write(0x7F); // Th контроль температуры макс 128грд
    ds.write(0xFF); //Tl контроль температуры мин -128грд
    ds.write(0x60); // 0x60 12-бит разрешение, 0x00 -9бит разрешение
    return;
  }

 if (OneWire::crc8(addr, 7) != addr[7]) {
     Serial.println("CRC is not valid!");
      return 1 ;
  }
 else{
//  Serial.println("CRC is valid!");
   return 0 ;
 }



}

//2. читаем температуру Функц
int read_temperatur(){
  ds.reset();
  ds.select(addr);
//  ds.write(0xCC, 1);        // Команда пропуск ROM.
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad Команда чтения памяти датчика. Команда используется для чтения всех 9ти байтов памяти DS18B20
 
 for ( i = 0; i < 9; i++) {           //читаем 9 байт
    data[i] = ds.read();
  } 
 // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.

  int16_t raw = (data[1] << 8) | data[0];

  
  celsius = (float)raw / 16.000;
  temperature = (double)raw / 16.000; 
  Serial.print(raw,BIN);
  Serial.print("  ");
  Serial.print(celsius);
  Serial.print("  ");
  Serial.print(temperature);
  Serial.print("  ");
  Serial.print(data[4],HEX);
  


  Serial.println(" ");
  
}

