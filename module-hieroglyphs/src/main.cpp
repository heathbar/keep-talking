#define __AVR__ 1 // cheat codes
#define __AVR_ATmega328P__ 1

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <button.h>
#include <chat.h>
#include "glyph.h"
#include "quotes.h"


// #define LCD_CS A3 // Chip Select
// #define LCD_CD A2 // Command/Data
// #define LCD_WR A1 // LCD Write
// #define LCD_RD A0 // LCD Read (unused in this sketch)
// #define LCD_RESET A6 // Nonsense, the pin is connected to Arduino reset, not GPIO

#define LCD_CS 10 // Chip Select
#define LCD_CD 11 // Command/Data
#define LCD_WR 12 // LCD Write
#define LCD_RD 13 // LCD Read (unused in this sketch)
#define LCD_RESET A6 // Nonsense, the pin is connected to Arduino reset, not GPIO

#define STATUS_RED A4
#define STATUS_GRN A5

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define INDICATOR 0x4208
#define	CORRECT  0x07E0
#define STRIKE 0xF800


Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

Chat chat(ChatSource::Hieroglyphs);
ChatMessage msg;

Button buttons[] = {
  Button(A0), // red
  Button(A2), // blue
  Button(A1), // green
  Button(A3)  // yellow
};

short solution[4];
short solution_scrambled[4];
short solution_index = 0;
bool disarmed = false;
bool detonated = false;
bool won = true;
long randomSeedAnalog;

void draw(short pos, Glyph glyph, uint16_t color);
void scramble(short *input, short *output);
void paint_indicator(short i, uint16_t color);
void reset();
void showQuote(short x);


void setup(void) {
  chat.begin();

  tft.begin(0x9341); // 0x9341 identifies the make/model of the LCD panel
  tft.fillScreen(BLACK);

  pinMode(A5, INPUT);
  randomSeedAnalog = analogRead(A5);

  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GRN, OUTPUT);

  digitalWrite(STATUS_RED, HIGH);
  digitalWrite(STATUS_GRN, LOW);
}


void loop(void) {

  if (chat.receive(&msg))
  {
    switch (msg.message) {
      case MessageType::Reset:
        randomSeed(randomSeedAnalog * msg.data);
        reset();
        break;

      case MessageType::Detonate:
        detonated = true;
      
        tft.fillScreen(RED);

        tft.fillRect(42, 92, 156, 60, BLACK);
        tft.drawRect(42, 92, 156, 60, WHITE);
        tft.setCursor(77, 100);
        tft.setTextSize(2);
        tft.setTextColor(WHITE);
        tft.println("SAD DAY");

        tft.setCursor(55, 125);
        tft.setTextColor(RED);
        tft.setTextSize(3);
        tft.println("FOR YOU");

        // tft.setCursor(0, 75);
        // tft.setTextSize(1);
        // tft.setTextColor(WHITE);

        // PGM_P const *movie = (PGM_P const *)pgm_read_dword(&(shows[x]));

        // for (int i = 1; i < 7; i++) {

        //   char buffer[strlen_P(movie[i])];
        //   strcpy_P(buffer, (char*)pgm_read_dword(&(movie[i])));
        //   tft.println(buffer);
        // }

        // tft.println();
        // tft.println();

        // tft.setTextColor(0xCE79);
        // tft.println(" From the movie ");
        // tft.println();

        // tft.setTextColor(GREEN);
        // char title[strlen_P(movie[0])];
        // strcpy_P(title, (char*)pgm_read_dword(&(movie[0])));
        // tft.println(title);
        // tft.println();

      case MessageType::Win:
        won = true;
        break;

    }
  }

  if (!disarmed && !detonated && !won)
  {
    for (short i = 0; i < 4; i++)
    {
      if (buttons[i].isPressed())
      {
        short glyph_pressed = solution_scrambled[i];
        short correct_glyph = solution[solution_index];

        if (glyph_pressed == correct_glyph)
        {
          paint_indicator(i, CORRECT);
          solution_index++;
        }
        else
        {
          paint_indicator(i, STRIKE);
          chat.send(MessageType::Strike);
          delay(100);
          paint_indicator(0, INDICATOR);
          paint_indicator(1, INDICATOR);
          paint_indicator(2, INDICATOR);
          paint_indicator(3, INDICATOR);
          solution_index = 0;
        }
      }
    }

    if (solution_index == 4)
    {
      tft.fillScreen(BLACK);
      disarmed = true;

      // tft.setCursor(0, 0);
      // tft.setTextSize(1);
      // tft.setTextColor(CORRECT);
      
      // for (short i = 0; i < 28; i++)
      // {
      //   char buffer[strlen_P(code[i])];
      //   strcpy_P(buffer, (PGM_P)pgm_read_word(&code[i]));
      //   tft.println(buffer);
      // }
      
      tft.fillRect(42, 92, 156, 60, BLACK);
      tft.drawRect(42, 92, 156, 60, WHITE);
      tft.setCursor(70, 100);
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      tft.println("SEQUENCE");

      tft.setCursor(50, 125);
      tft.setTextColor(GREEN);
      tft.setTextSize(3);
      tft.println("ACCEPTED");

      chat.send(MessageType::Disarm);

      digitalWrite(STATUS_RED, LOW);
      digitalWrite(STATUS_GRN, HIGH);
    }
  }
}

void paint_indicator(short i, uint16_t color)
{
    switch(i) 
    {
      case 0:
        tft.fillRoundRect(16, 136, 88, 8, 4, color);
        break;
      case 1:
        tft.fillRoundRect(136, 136, 88, 8, 4, color);
        break;
      case 2:
        tft.fillRoundRect(16, 296, 88, 8, 4, color);
        break;
      case 3:
        tft.fillRoundRect(136, 296, 88, 8, 4, color);
        break;
    }
}

void draw(short pos, Glyph glyph, uint16_t color)
{
  switch (pos)
  {
    case 0:
      tft.drawBitmap(glyph.x, glyph.y, glyph.data, glyph.w, glyph.h, color);
      break;
    case 1:
      tft.drawBitmap(glyph.x + 120, glyph.y, glyph.data, glyph.w, glyph.h, color);
      break;
    case 2:
      tft.drawBitmap(glyph.x, glyph.y + 160, glyph.data, glyph.w, glyph.h, color);
      break;
    case 3:
      tft.drawBitmap(glyph.x + 120, glyph.y + 160, glyph.data, glyph.w, glyph.h, color);
      break;
  }
  paint_indicator(pos, INDICATOR);
}

void scramble(short *input, short *output)
{
  // copy
  output[0] = input[0];
  output[1] = input[1];
  output[2] = input[2];
  output[3] = input[3];

  // shuffle
  for (int i = 0; i < 4; i++)
  {
    int j = random(0, 4 - i);
    int t = output[i];
    output[i] = output[j];
    output[j] = t;
  }
}

void reset()
{
  tft.fillScreen(BLACK);

  // select a random group
  Glyph *group = glyph_groups[random(6)];
  
  // extract 4 random glyphs (in order) from the 7 in the group
  const short n = 7;
  const short m = 4;
  short in, im = 0;

  for (in = 0; in < n && im < m; in++)
  {
    short rn = n - in;
    short rm = m - im;
    if (random() % rn < rm)
    {
      solution[im++] = in;
    }
  }

  // copy/scramble the 4 glyphs
  scramble(solution, solution_scrambled);

  // draw the glyphs in scrambled order
  draw(0, group[solution_scrambled[0]], RED);
  draw(1, group[solution_scrambled[1]], BLUE);
  draw(2, group[solution_scrambled[2]], GREEN);
  draw(3, group[solution_scrambled[3]], YELLOW);

  solution_index = 0;
  disarmed = false;
  detonated = false;
  won = false;

  digitalWrite(STATUS_RED, LOW);
  digitalWrite(STATUS_GRN, HIGH);
}
