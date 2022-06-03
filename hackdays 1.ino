
//==============CONFIG AREA==============
const int setup_minlightlevel = 40;
const int setup_distance = 5;
int setupvar = 0; // If set to 1, startup animation will not be displayed
//=======================================

const int usInput = 2;  // Ultraschallsensor
const int tlInput = 3;  // Temperatur- und Luftfeuchtigkeitssensor
const int btnInput = 4; // Button
// liInput auf A0

const int sumOutput = 5;  // Summer
const int vibOutput = 6;  // Vibrationsmotor
const int lcdOutput = 12; // LCD-Display

int dis;  // Distance
int temp; // Temperature
int hum;  // Humidity

#include "Ultrasonic.h"
Ultrasonic ultrasonic(usInput);

#include "DHT.h"      // include adafruit DHT library
#define DHTTYPE DHT11 // DHT 11
DHT dht(tlInput, DHTTYPE);

#include "rgb_lcd.h"
#include <Wire.h>
rgb_lcd lcd;

unsigned long zeitstempel = 0;

int timeInSeconds = 0;
int timeInMinutes = 0;

bool timerIsRunning = false;

bool windowIsOpen;

bool alarmIsRunning = true;

int lightlevel; // mapped lightlevel

// bool timeoutAllowed;

int timeout = 60;

bool isDark;

bool ecoHasChanged = true;

void setup()
{

  getValues();

  pinMode(8, OUTPUT); // TEMP LED OUTPUT - On if Roomlights should be on

  pinMode(usInput, INPUT);
  pinMode(tlInput, INPUT);
  pinMode(btnInput, INPUT);
  pinMode(A0, INPUT);

  pinMode(sumOutput, OUTPUT);
  pinMode(vibOutput, OUTPUT);

  Serial.begin(9600);
  zeitstempel = millis();

  // if (dis > setup_distance) {
  //   setTimer(5, 0);
  //   windowIsOpen = true; // Fenster Offen
  // }
  // if (dis <= setup_distance) {
  //   setTimer(20, 0);
  //   windowIsOpen = false; // Fenster Geschlosesen
  // }

  lcd.begin(16, 2);
  lcd.setRGB(0, 0, 0);
  lcd.clear();
  delay(200);
}

void loop()
{

  if (setupvar == 0)
  {
    startupanim();
    setupvar = 1;
  }

  if (isDark && timeout <= 0)
  {
    lcdprint("", "");
    lcd.setRGB(0, 0, 0);
    timerIsRunning = false;
  }

  else if (isDark == false || timeout > 0)
  {
    getValues();
    if (dis > setup_distance && ecoHasChanged)
    {
      setTimer(0, 10);
      windowIsOpen = true; // Fenster Offen
      ecoHasChanged = false;
    }
    if (dis <= setup_distance && ecoHasChanged)
    {
      setTimer(0, 20);
      windowIsOpen = false; // Fenster Geschlosesen
      ecoHasChanged = false;
    }
  }

  if (digitalRead(btnInput) == HIGH)
  {
    timeout = 60;
    lcd.setRGB(255, 255, 255);
    ecoHasChanged = true;
  }

  if (dis > setup_distance && windowIsOpen == false)
  {
    setTimer(0, 10);
    alarmIsRunning = true;
    windowIsOpen = true;
    lcd.setRGB(255, 255, 255);
  }
  if (dis <= setup_distance && windowIsOpen == true)
  {
    setTimer(0, 20);
    alarmIsRunning = true;
    windowIsOpen = false;
    lcd.setRGB(255, 255, 255);
  }

  if (zeitstempel + 1000 < millis())
  {
    zeitstempel = millis();

    timeout--;

    if (timerIsRunning)
    {
      timeInSeconds--;

      if (timeInSeconds < 0)
      {
        timeInSeconds = 59;
        timeInMinutes--;
      }
      Serial.print(timeInMinutes);
      Serial.print(":");
      Serial.println(timeInSeconds);
    }
    if (timeInSeconds < 10)
    {
      lcdprint("Time:" + String(timeInMinutes) + ":0" + String(timeInSeconds),
               "Temp:" + String(temp) + "C " + "Hum:" + String(hum) + "%");
    }
    else
    {
      lcdprint("Time:" + String(timeInMinutes) + ":" + String(timeInSeconds),
               "Temp:" + String(temp) + "C " + "Hum:" + String(hum) + "%");
    }
    if (timeInMinutes < 10)
    {
      lcdprint("Time:0" + String(timeInMinutes) + ":" + String(timeInSeconds),
               "Temp:" + String(temp) + "C " + "Hum:" + String(hum) + "%");
    }
    else
    {
      lcdprint("Time:" + String(timeInMinutes) + ":" + String(timeInSeconds),
               "Temp:" + String(temp) + "C " + "Hum:" + String(hum) + "%");
    }
    if (timeInMinutes < 10 && timeInSeconds < 10)
    {
      lcdprint("Time:0" + String(timeInMinutes) + ":0" + String(timeInSeconds),
               "Temp:" + String(temp) + "C " + "Hum:" + String(hum) + "%");
    }

    if (timeInSeconds == 0 && timeInMinutes == 0)
    {
      timerDone();
      timerIsRunning = false;
    }
  }
}

void getValues() // circa 250 bis 1000
{

  //  Serial.println("=======================");
  lightlevel = map(analogRead(A0), 0, 762, 0, 100);
  // Serial.println(lightlevel);
  if (lightlevel > setup_minlightlevel)
  {
    // Serial.println("Licht ist an.");
    digitalWrite(8, HIGH);
    isDark = false;
  }
  else
  {
    digitalWrite(8, LOW);
    isDark = true;
    // Serial.println("Licht ist aus.");
  }

  // --------------------------
  dis = ultrasonic
            .MeasureInCentimeters(); // two measurements should keep an interval
  temp = dht.readTemperature();
  hum = dht.readHumidity();
}

void setTimer(int min, int sec)
{
  timeInMinutes = min;
  timeInSeconds = sec;
  timerIsRunning = true;
  alarmIsRunning = true;
}

void timerDone()
{
  if (alarmIsRunning)
  {
    lcd.setRGB(255, 0, 0);
    for (int i = 0; i <= 5; i++)
    {
      if (i % 2 == 0)
      {
        digitalWrite(sumOutput, HIGH);
        digitalWrite(vibOutput, HIGH);
        delay(250);
      }
      else
      {
        digitalWrite(sumOutput, LOW);
        digitalWrite(vibOutput, LOW);
        delay(250);
      }
    }
    alarmIsRunning = false;
  }
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
    lcdprint("",
             "Temp:" + String(temp) + "C " + "Hum:" + String(hum) + "%");
  }
}

void startupanim()
{
  // ANIM
  int maindelay = 150; // Delay between moves at name screen
  int valuedelay = 50; // Delay between chars at value screen
  lcd.clear();
  String productaname = "Covid-Pro";
  lcd.setRGB(255, 255, 255);
  lcdprint(productaname, "RinderWieSe&Klickermann");
  delay(1000);
  lcdprint(productaname, "inderWieSe&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "nderWieSe&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "derWieSe&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "erWieSe&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "rWieSe&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "WieSe&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "ieSe&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "eSe&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "Se&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "e&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "&Klickermann");
  delay(maindelay);
  lcdprint(productaname, "Klickermann");
  delay(1000);
  lcdprint("", "T");
  delay(valuedelay);
  lcdprint("", "Te");
  delay(valuedelay);
  lcdprint("", "Tem");
  delay(valuedelay);
  lcdprint("", "Temp");
  delay(valuedelay);
  lcdprint("", "Temp:");
  delay(valuedelay);
  lcdprint("", "Temp:-");
  delay(valuedelay);
  lcdprint("", "Temp:--");
  delay(valuedelay);
  lcdprint("", "Temp:--C");
  delay(valuedelay);
  lcdprint("", "Temp:--C ");
  delay(valuedelay);
  lcdprint("", "Temp:--C H");
  delay(valuedelay);
  lcdprint("", "Temp:--C Hu");
  delay(valuedelay);
  lcdprint("", "Temp:--C Hum");
  delay(valuedelay);
  lcdprint("", "Temp:--C Hum:");
  delay(valuedelay);
  lcdprint("", "Temp:--C Hum:-");
  delay(valuedelay);
  lcdprint("", "Temp:--C Hum:--");
  delay(valuedelay);
  lcdprint("", "Temp:--C Hum:--%");
  delay(valuedelay);
  delay(1500);
}