#include <Arduino.h>

#define LCD Serial2

void writeLCD(byte b)
{
  LCD.write(b);
  delay(15);
}

void printLCD(String s)
{
  LCD.print(s);
  for (short i = String(s).length(); i < 16; i++)
  {
    LCD.print(" ");
  }
  delay(15);
}

void clearLCD()
{
  LCD.write(0xFE);   //command flag
  LCD.write(0x01);   //clear command.
  delay(15);
}

void backlightOnLCD()
{
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(157);    //light level.
  delay(15);
}

void backlightOffLCD()
{
    LCD.write(0x7C);   //command flag for backlight stuff
    LCD.write(128);     //light level for off.
    delay(15);
}

void selectLineOneLCD()
{
   LCD.write(0xFE);   //command flag
   LCD.write(128);    //position
   delay(15);
}

void selectLineTwoLCD()
{
   LCD.write(0xFE);   //command flag
   LCD.write(192);    //position
   delay(15);
}