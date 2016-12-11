//Программа должна будет сканировать температуру  и отправлять показания
//SMS при понижении температуры ниже чем +2 градуса
//и звонить при понижении ниже чем -2 градуса

//содержание СМС: 
//дата.время - температура //// "29.12.2016 15:54 = -2 Celcia"

#include <Wire.h> 
#include <OneWire.h>
//#include <Time.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

//инициализируем переменные
OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary) - датчик температуры DS18B20

byte i; //служебная переменная для циклов
byte addr[8];  // для адреса 18B20
String data_1;  //

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("");
  Serial.println("------- Begin Runer  ----------");
}

void loop() {
//1. ищем датчик вызов
search_temperatur();

//2. читаем температуру Функц
//read_temperatur();
Serial.println(read_temperatur(),4); //для отладки

//3. читаем время 
read_data_time();

Serial.println("yy " + data_1); //для отладки



}
//конец loop()
//==============================================================================
//==============================================================================


// для вспомогательных функции

//1. ищем датчик температуры функция
int search_temperatur(){                //возврашает 1 если нашел датчик , 0 если не нашел.
if ( !ds.search(addr)) {
//  Serial.println("No more addresses.");
    ds.reset_search();
    delay(250);
// снять все ремарки если нужно поменять разрешение 
    ds.reset(); // сброс шины
    ds.select(addr);// выставить адрес
    ds.write(0x4E); // разрешение записать конфиг
    ds.write(0x7F); // Th контроль температуры макс 128грд
    ds.write(0xFF); // Tl контроль температуры мин -128грд
    ds.write(0x60); // 0x60 12-бит разрешение, 0x00 -9бит разрешение
    return;
}

 if (OneWire::crc8(addr, 7) != addr[7]) {
//     Serial.println("CRC is not valid!"); //
      return 0 ;
 }
 else{
//  Serial.println("CRC is valid!");
   return 1 ;
 }
}

//2. читаем температуру Функц
float read_temperatur(){
// локальные переменные для функции
byte data[12]; // для данных 18B20

float celsius;  // измеренная температура
  
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
//data[0] = 0x5E ; 
//data[1] = 0xFF ;
  int16_t raw = (data[1] << 8) | data[0];
  celsius = (float)raw / 16.000;
//  Serial.print(celsius,4);
//  Serial.println(" ");
return celsius ;  
}

//3. читаем дату время из таймера
int  read_data_time() {
data_1 = "";
  tmElements_t tm;
RTC.read(tm);
data_1 = print2digits(tm.Day) +'.'+ print2digits(tm.Month)+ '.' + tmYearToCalendar(tm.Year) +" " + print2digits(tm.Hour) + ':'+ print2digits(tm.Minute)+':'+ print2digits(tm.Second);

//data_1 = tm.Second;

/* 
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
 
 */  


//data_1 = "03.12.2016 14:18" ;
return 1;



}


//4. функция пишет два символа если одна цифра 
String print2digits(int number) {
String str1 = "";

  if (number >= 0 && number < 10) {
    str1 = '0'; 
    str1 = str1 + (String)number;  
//    Serial.write('0');
  }
else {  
 str1 = (String)number;
}
//Serial.println(str1); //для отладки
return str1 ;
}


