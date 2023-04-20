#ifndef ENCODER_H
#define ENCODER_H

class Encoder
{
  private:
    short clockPin;
    short dataPin;
    bool last;

  public:
    Encoder(short clockPin, short dataPin);
    short read();

};


#endif