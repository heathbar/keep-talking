#include "Arduino.h"
#include "ColorButton.h"
#include "MusicNotes.h"

#define debounceTime 25

using namespace std;

ColorButton::ColorButton(char id, short ledPin, short btnPin, int tone)
{
  this->id = id;
  this->ledPin = ledPin;
  this->btnPin = btnPin;
  this->toneHertz = tone;
  this->isOn = false;
  this->btnState = true;
  this->lastState = false;
  this->lastDebounceTime = 0;

  pinMode(btnPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void ColorButton::turnOn()
{
  if (!this->isOn) {
    this->isOn = true;
    digitalWrite(this->ledPin, HIGH);
    tone(piezoPin, this->toneHertz);
  }
}


void ColorButton::turnOnMuted()
{
  if (!this->isOn) {
    this->isOn = true;
    digitalWrite(this->ledPin, HIGH);
  }
}

void ColorButton::turnOff()
{
  if (this->isOn) {
    this->isOn = false;
    digitalWrite(this->ledPin, LOW);
    noTone(piezoPin);
  }
}

void ColorButton::refresh()
{
  this->isPressed = false;
  this->isReleased = false;

  int reading = digitalRead(this->btnPin);
  if (reading != this->lastState)
  {
    this->lastDebounceTime = millis();
  }

  if ((millis() - this->lastDebounceTime) > debounceTime)
  {
    if (reading != this->btnState)
    {
      this->btnState = reading;

      if (this->btnState == LOW)
      {
        this->isPressed = true;
      }
      else
      {
        this->isReleased = true;
      }
    }
  }
  this->lastState = reading;
}
