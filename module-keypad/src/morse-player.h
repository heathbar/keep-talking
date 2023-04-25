#ifndef MORSE_PLAYER_H
#define MORSE_PLAYER_H

#include <Arduino.h>
#include <Tone.h>

#define REST_LENGTH 200
#define PAUSE_LENGTH 200
#define DOT_LENGTH 200
#define DASH_LENGTH 600

class MorsePlayer
{
  private:
    Tone *tone;
    String phrase;
    short index;
    long start;
    long duration;
    int character_durations[96];

    void computeDurations();

  public:
    MorsePlayer(Tone *tone);
    void setPhrase(String phrase);
    void tick();
};

#endif