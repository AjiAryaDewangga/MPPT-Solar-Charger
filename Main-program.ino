/*
   contact: ajiaryad@gmail.com
*/
//lcd
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
//arus
#include "ACS712.h"
ACS712  ACSin(A0, 5.0, 1023, 100);
float realArusIn = 0.0;
float ArusIn = 0.0;
ACS712  ACSout(A2, 5.0, 1023, 66);
float realArusOut = 0.0;
float ArusOut = 0.0;
float dayaIn = 0.0;
float dayaOut = 0.0;
//tegangan
#include <Wire.h>
float tegRealIn = 0.0;
float teganganIn = 0.0;
float tegRealOut = 0.0;
float teganganOut = 0.0;
//
float Power_anc = 0, voltage_anc = 0;
float delta = 1;
float pwm = 50;
//
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter filterArusIn(100, 1, 0.01);
SimpleKalmanFilter filterTegIn(10, 1, 0.01);
SimpleKalmanFilter filterArusOut(100, 1, 0.01);
SimpleKalmanFilter filterTegOut(10, 1, 0.01);
//

float val_pwm = 0.0;
float val_eff = 0.0;

byte PV[] = {
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B00000
};
byte PWM[] = {
  B11101,
  B10101,
  B10101,
  B10101,
  B10101,
  B10101,
  B10111,
  B00000
};
byte BAT[] = {
  B01110,
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B00000
};
byte EFF[] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100
};
void setup() {
  Serial.begin(1200);
  TCCR0B = TCCR0B & B11111000 | B00000001; // atur frekuensi PWM
  pinMode(6, OUTPUT);
  lcd.begin();
  lcd.createChar(0, PV);
  lcd.createChar(1, PWM);
  lcd.createChar(2, BAT);
  lcd.createChar(3, EFF);
}
int i = 0;

void loop() {
  readArusIn();
  readArusOut();
  readTeganganIn();
  readTeganganOut();
  readDayaIn();
  readDayaOut();
  sendGUI();
  i++;
  if (i > 10) {
    i = 0;

    if (dayaIn > Power_anc)
    { if (teganganIn > voltage_anc)
        pwm = pwm - delta;
      else
        pwm = pwm + delta;
    }
    else
    {
      if (teganganIn > voltage_anc)
        pwm = pwm + delta;
      else
        pwm = pwm - delta;
    }
    Power_anc = dayaIn;
    voltage_anc = teganganIn;
    if (pwm < 0)
      pwm = 0;
    if (pwm > 250)
      pwm = 250;

    analogWrite(6, pwm);
    tampilLCD();
  }
}

void tampilLCD() {
  lcd.backlight();

  //PV
  lcd.setCursor(1, 0);
  lcd.print("PV ");
  lcd.write(0);
  lcd.setCursor(0, 1);
  lcd.print(teganganIn, 2);
  if (teganganIn < 10) lcd.print("V ");
  else lcd.print("V");
  lcd.setCursor(0, 2);
  lcd.print(ArusIn, 2);
  lcd.print("A");
  lcd.setCursor(0, 3);
  lcd.print(dayaIn, 2);
  if (dayaIn < 10) lcd.print("W ");
  else lcd.print("W");
  //Pembatas
  lcd.setCursor(6, 0);
  lcd.print("|");
  lcd.setCursor(6, 1);
  lcd.print("|");
  lcd.setCursor(6, 2);
  lcd.print("|");
  lcd.setCursor(6, 3);
  lcd.print("|");
  //MPPT
  lcd.setCursor(7, 0);
  lcd.print("PWM ");
  lcd.write(1);
  lcd.setCursor(7, 1);
  val_pwm = 100 - (pwm / 254.0 * 100.0);
  lcd.print(val_pwm, 0);
  lcd.print(" %  ");
  lcd.setCursor(7, 2);
  lcd.print("Eff ");
  lcd.write(3);
  lcd.setCursor(7, 3);
  if (dayaOut > 0 && dayaIn > 0) {
    val_eff = constrain((dayaOut / dayaIn * 100.0), 0, 100);
  }
  lcd.print(val_eff, 0);
  lcd.print(" %  ");
  //Pembatas
  lcd.setCursor(13, 0);
  lcd.print("|");
  lcd.setCursor(13, 1);
  lcd.print("|");
  lcd.setCursor(13, 2);
  lcd.print("|");
  lcd.setCursor(13, 3);
  lcd.print("|");
  //Baterai
  lcd.setCursor(14, 0);
  lcd.print("BAT ");
  lcd.write(2);
  lcd.setCursor(14, 1);
  lcd.print(teganganOut, 2);
  if (teganganOut < 10) lcd.print("V ");
  else lcd.print("V");
  lcd.setCursor(14, 2);
  lcd.print(ArusOut, 2);
  lcd.print("A");
  lcd.setCursor(14, 3);
  lcd.print(dayaOut, 2);
  if (dayaOut < 10) lcd.print("W ");
  else lcd.print("W");
}

void readArusIn()
{
  realArusIn = ACSin.mA_DC();
  float measured_value = realArusIn + random(-2000, 2000) / 2000.0;
  float estimated_value = filterArusIn.updateEstimate(measured_value);

  ArusIn = constrain((estimated_value / 1000), 0, 20);
  //  Serial.print(ArusIn, 2);
  //  Serial.print(" A | ");
}
void readArusOut()
{
  realArusOut = ACSout.mA_DC();
  float measured_value3 = realArusOut + random(-2000, 2000) / 2000.0;
  float estimated_value3 = filterArusOut.updateEstimate(measured_value3);

  ArusOut = constrain((estimated_value3 / 1000), 0, 20);
  //  Serial.print(ArusOut, 2);
  //  Serial.print(" A | ");
}
void readTeganganIn()
{
  float temp;
  tegRealIn = analogRead(1);
  float measured_value2 = tegRealIn + random(-1023, 1023) / 1023.0;
  float estimated_value2 = filterTegIn.updateEstimate(measured_value2);

  temp = estimated_value2 / 4.092;
  tegRealIn = temp;
  teganganIn = constrain(((tegRealIn) * 10) / 100, 0, 25);

  //  Serial.print((String)teganganIn + " V | ");
}
void readTeganganOut()
{
  float temp2;
  tegRealOut = analogRead(A3);
  float measured_value4 = tegRealOut + random(-1023, 1023) / 1023.0;
  float estimated_value4 = filterTegOut.updateEstimate(measured_value4);
  teganganOut = (tegRealOut / 1023 * 4.092) * 1220 / 220;

  //  Serial.print((String)teganganOut + " V | ");
}
void readDayaIn()
{
  dayaIn = ArusIn * teganganIn;
  //  Serial.println((String)dayaIn + " W");
}
void readDayaOut()
{
  dayaOut = ArusOut * teganganOut;
  //  Serial.println((String)dayaOut + " W");
}

void sendGUI() {
  Serial.print(teganganIn, 2), Serial.println ('a');
  Serial.print(ArusIn, 2), Serial.println ('b');
  Serial.print(dayaIn, 2), Serial.println ('c');
  if (ArusOut > 0) Serial.print("ON"), Serial.println ('d');
  else Serial.print("OFF"), Serial.println ('d');
  Serial.print(val_pwm, 0), Serial.println ('e');
  Serial.print(val_eff, 0), Serial.println ('f');
  Serial.print(teganganOut, 2), Serial.println ('g');
  Serial.print(ArusOut, 2), Serial.println ('h');
  Serial.print(dayaOut, 2), Serial.println ('i');

}
