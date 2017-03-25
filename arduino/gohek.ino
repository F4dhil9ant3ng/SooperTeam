/*
  Modul sooperkey dibuat dalam rangka GO-HACKATON
  oleh:
  dwi.setiyadi@gmail.com
  suparman.elmizan@gmail.com
  25 - 26 Maret 2017
*/
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(11, 10);
char junk;
String command = "";
const int switchon = 9;
const int switchstart = 6;
const int switchbuzz = 5;
const int sensorgetar = 12;
int ign = 0;
int bukanpakogah = 0;
int timerpakogah = 0;

void setup()
{
  bluetooth.begin(9600);
  pinMode(sensorgetar, INPUT);
  pinMode(switchon, OUTPUT);
  pinMode(switchstart, OUTPUT);
  pinMode(switchbuzz, OUTPUT);

  analogWrite(switchon, LOW);
  analogWrite(switchstart, LOW);
  analogWrite(switchbuzz, LOW);
}

void loop()
{
  if (bluetooth.available()) {
    while (bluetooth.available())
    {
      char inChar = (char)bluetooth.read();
      command += inChar;
    }
    while (bluetooth.available() > 0)
    {
      junk = bluetooth.read() ;
    }
    if (command == "a") {
      analogWrite(switchon, HIGH);
      ign = 1;
      bluetooth.println("kontak on");
    } else if (command == "b") {
      if (ign == 0) {
        analogWrite(switchon, HIGH);
        delay(2500);
      }
      analogWrite(switchstart, HIGH);
      delay(1000);
      analogWrite(switchstart, LOW);
      bluetooth.println("engine on");
    } else if (command == "c") {
      analogWrite(switchon, LOW);
      ign = 0;
      bluetooth.println("mati");
    } else if (command == "d") {
      bipbip(3, 500);
    }
    command = "";
  }
  int sensorgetarStatus = digitalRead(sensorgetar);
  if (sensorgetarStatus == 1 && ign == 0)
  {
    analogWrite(switchon, LOW);
    bipbip(60, 500);
  }
}

void bipbip(int times, int delays)
{
  int i = 0;
  for (i; i < times; i++)
  {
    analogWrite(switchbuzz, HIGH);
    delay(delays);
    analogWrite(switchbuzz, LOW);
    delay(delays);

    if (bluetooth.available()) {
      while (bluetooth.available())
      {
        char inChar = (char)bluetooth.read();
        command += inChar;
      }
      while (bluetooth.available() > 0)
      {
        junk = bluetooth.read() ;
      }
      if (command == "c") {
        analogWrite(switchon, LOW);
        bukanpakogah = 0;
        ign = 0;
        command = "";
        bluetooth.println("mati");
        break;
      }
      command = "";
    }
  }
}
