#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int usInput = 2;
const int tlInput = 3;
const int btnInput = 4;

const int sumOutput = 5;
const int vibOutput = 6;
const int lcdOutput = 12;

long dis;                  // distance
int temp = random(15, 26); // temperature
int hum = random(15, 99);  // humidity
//========================================

int disR = 255;
int disG = 255;
int disB = 255;

int i = 0;

String line1 = "Welcome";
String line2 = "User";

void setup()
{
  lcd.begin(16, 2);
  lcd.setRGB(0, 0, 0);
  delay(100);
  lcdprint("Starting...", " ");
  delay(550);
  lcd.clear();
  delay(100);
  for (i = 0; i < 255; i++)
  {
    lcd.setRGB(i, i, i);
    delay(2);
  }
  lcdprint("", "T");
  delay(100);
  lcdprint("", "Te");
  delay(100);
  lcdprint("", "Tem");
  delay(100);
  lcdprint("", "Temp");
  delay(100);
  lcdprint("", "Temp:");
  delay(100);
  lcdprint("", "Temp:-");
  delay(100);
  lcdprint("", "Temp:--");
  delay(100);
  lcdprint("", "Temp:--C");
  delay(100);
  lcdprint("", "Temp:--C ");
  delay(100);
  lcdprint("", "Temp:--C H");
  delay(100);
  lcdprint("", "Temp:--C Hu");
  delay(100);
  lcdprint("", "Temp:--C Hum");
  delay(100);
  lcdprint("", "Temp:--C Hum:");
  delay(100);
  lcdprint("", "Temp:--C Hum:-");
  delay(100);
  lcdprint("", "Temp:--C Hum:--");
  delay(100);
  lcdprint("", "Temp:--C Hum:--%");
  delay(100);

  // Vllt Leerzeichen oder direkt die richtigen Zahlen
}

void loop()
{
  lcdshow("temp");
  delay(1000);
  // lcdprint("einszweidreivierf","1");
  // delay(1000);
}

void lcdprint(String line1, String line2)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void lcdshow(String item)
{
  if (item == "temp")
  {
    lcdprint("", "Temp:" + String(temp) + "C " + "Hum:" + String(hum) + "%");
  }
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/