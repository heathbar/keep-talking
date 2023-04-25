#include <Arduino.h>
#include <Tone.h>
#include "morse-player.h"


MorsePlayer::MorsePlayer(Tone *tone): tone(tone)
{
}

/**
 * Limited to 31 character phrase
*/
void MorsePlayer::setPhrase(String phrase)
{
  start = 0;
  this->phrase = String("    ") + phrase; 
  computeDurations();
}


void MorsePlayer::tick()
{
  if (start == 0)
  {
    start = millis();
    index = -1;
  }

  const long now = millis();
  const long elapsed = (now - start) % duration;
  long progress = elapsed;

  short i = 0;
  while (progress > character_durations[i])
  {
    progress -= character_durations[i];
    i++;
  }

  if (i != index)
  {
    index = i;
    short half_i = i/2;

    if (index % 2 == 0 && (phrase[half_i] == '.' || phrase[half_i] == '-'))
    {
      tone->play(262);
    }
    else
    {
      tone->stop();
    }
  }
}

void MorsePlayer::computeDurations()
{
  duration = 0;
  
  for (short i = 0; i < phrase.length(); i++)
  {
    if (phrase[i] == '-')
    {
      duration += DASH_LENGTH + REST_LENGTH;

      character_durations[i*2] = DASH_LENGTH;
      character_durations[i*2+1] = REST_LENGTH;
    }
    else if (phrase[i] == '.')
    {
      duration += DOT_LENGTH + REST_LENGTH;
      character_durations[i*2] = DOT_LENGTH;
      character_durations[i*2+1] = REST_LENGTH;
    }
    else if (phrase[i] == ' ')
    {
      duration += PAUSE_LENGTH + REST_LENGTH;
      character_durations[i*2] = PAUSE_LENGTH;
      character_durations[i*2+1] = REST_LENGTH;
    }
  }
}
