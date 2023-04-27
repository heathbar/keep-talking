#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Alphanumeric_Display.h>
#include "password-component.h"
#include <chat.h>
#include <button.h>

const short ledPin = 2;
const short gndPin = 7;
const short btnPin = A3;
const short gndPin2 = 13;
const short armedPin = A0;
const short disarmedPin = A1;

HT16K33 display;

PasswordComponent *password;
Button submit(btnPin);

const char *database[][5] = {
    {"BACK", "PJMET", "ELYHR", "SNPIB", "GCDWV"},
    {"BOMB", "DLKFA", "ZGICH", "LADNS", "FRPTJ"},
    {"CAMP", "LPDSO", "UZQRH", "OQPLK", "TAJCH"},
    {"CENT", "LPBSO", "UZQRH", "OQPLK", "FAJCH"},
    {"CHIP", "NAMES", "AEOUV", "BWSOK", "ERSTF"},
    {"DENT", "AEHWN", "ZABLV", "JPMBC", "ATLDQ"},
    {"DOME", "ROMAN", "ACFHU", "NIJKV", "FLSCD"},
    {"EZRA", "SDKUA", "WAJOK", "TSMEX", "YFPMB"},
    {"FIRE", "TIHGJ", "GAPUD", "VEYCL", "PTXSN"},
    {"GAME", "JOQDT", "IUSPR", "NBSPX", "ABCDF"},
    {"GOLF", "PIELX", "GCHIR", "NMAID", "WATER"},
    {"GONE", "HSYOK", "EMTZS", "BIWPG", "ASCIT"},
    {"HIGH", "STWAU", "DHOFL", "FHIJK", "OMBAN"},
    {"HEAR", "YNAFJ", "KRONZ", "CLIQU", "HMAET"},
    {"HELP", "FNIRT", "OSJDW", "HKMPR", "UWEBT"},
    {"LAMB", "KODER", "TUVWB", "SNACY", "CDEFG"},
    {"LEAP", "MOREF", "YTCKV", "BARED", "COGAN"},
    {"MELT", "SNACK", "LUNCH", "BFAST", "DINER"},
    {"MINE", "SLXOU", "UVRYC", "AMPIJ", "LBRAD"},
    {"MINT", "GOCUJ", "ALDBN", "ERXVI", "QBMSL"},
    {"NEXT", "TOADZ", "IPSVK", "JVLCR", "BURNY"},
    {"PALM", "TRIMO", "ECUWF", "JBDYI", "KUFPS"},
    {"PASS", "JIMBO", "OIKWD", "FLOTE", "SCOPY"},
    {"PREY", "SHOEL", "HXFBI", "AUOPT", "VFNWJ"},
    {"RAMP", "LEGUF", "HIYHR", "KNOCV", "IDCKA"},
    {"SEND", "PGOAT", "DOIBQ", "UVRAY", "BIPMO"},
    {"SHIP", "FOUNT", "XLEKG", "BLAMP", "SHADE"},
    {"SIDE", "MATEY", "CWONG", "DIRTY", "LOQPS"},
    {"TENT", "BIGXL", "TINYS", "AVCLB", "DINOZ"},
    {"XRAY", "YZQGB", "OHSIK", "UNDER", "ABOVE"}};

Chat chat(ChatSource::Password);
ChatMessage msg;

bool disarmed = false;
bool detonated = false;
void reset();

void setup()
{
  randomSeed(analogRead(A2));
  chat.begin();
  Wire.begin();

  if (display.begin() == false)
  {
    while (1);
  }

  pinMode(ledPin, OUTPUT);
  pinMode(gndPin, OUTPUT);
  pinMode(gndPin2, OUTPUT);
  pinMode(armedPin, OUTPUT);
  pinMode(disarmedPin, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP);
  digitalWrite(ledPin, HIGH);
  digitalWrite(gndPin, LOW);
  digitalWrite(gndPin2, LOW);
  digitalWrite(armedPin, HIGH);
  digitalWrite(disarmedPin, LOW);

  short index = random(27);
  const char **args = database[index];
  password = new PasswordComponent(args);

}

void loop()
{
  if (chat.receive(&msg))
  {

    if (msg.message == MessageType::Reset)
    {
      reset();
    }
    else if (msg.message == MessageType::Detonate)
    {
      detonated = true;
      display.displayOn();
      display.print("UHOH");
      delay(200);
      display.displayOff();
      delay(200);
      display.displayOn();
      delay(200);
      display.displayOff();
      delay(200);
      display.displayOn();
      delay(200);
      display.displayOff();
      delay(200);
      display.print("BOOM");
      delay(100);
      display.displayOff();
      delay(100);
      display.displayOn();
      delay(100);
      display.displayOff();
      delay(100);
      display.displayOn();
      delay(100);
      display.displayOff();
    }
  }

  if (!detonated && !disarmed)
  {
    char pass[5];
    bool correct = password->eval(pass);
    display.print(pass);

    if (submit.isPressed())
    {
      if (correct)
      {
        // Serial.println("Disarmed!");
        chat.send(MessageType::Disarm);
        delete password;

        digitalWrite(armedPin, LOW);
        digitalWrite(disarmedPin, HIGH);

        display.displayOn();
        delay(200);
        display.displayOff();
        delay(200);
        display.displayOn();
        delay(200);
        display.displayOff();
        delay(200);
        display.displayOn();
        delay(200);
        display.displayOff();
        delay(200);

        disarmed = true;
      }
      else
      {
        chat.send(MessageType::Strike);
      }
    }
  }

}


void reset()
{
  detonated = false;
  disarmed = false;
  delete password;
  short index = random(27);
  const char **args = database[index];
  password = new PasswordComponent(args);
  password->mix();

  char pass[5];
  while (password->eval(pass))
  {
    password->mix();
  }
}