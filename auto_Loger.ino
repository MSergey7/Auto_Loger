//Программа должна будет сканировать температуру  и отправлять показания
//SMS при понижении температуры ниже чем +2 градуса
//и звонить при понижении ниже чем -2 градуса

//содержание СМС: 
//дата.время - температура //// "29.12.2016 15:54 = -2 Celcia"

#include <Wire.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("");
  Serial.println("-------------------");
}
