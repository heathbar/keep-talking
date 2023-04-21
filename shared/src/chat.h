#ifndef CHAT_H
#define CHAT_H

#include <Arduino.h>

enum class ChatSource {
  Debug,
  Main,
  SimonSays,
  Password,
  Buttons,
  Clock,
  Wires,
  Hieroglyphs,
  BinaryCounter,
  Maze,
  Keypad,
  Mastermind,
  TBD,
  LAST_VALUE
};

enum class MessageType 
{
  Debug,
  Reset,
  Disarm,
  Strike,
  Detonate,
  Difficulty,
  Time1,
  Time2,
  Time3,
  Time4,
  LAST_VALUE
};

const uint8_t DIFFICULTY_NO_FAIL = 0;
const uint8_t DIFFICULTY_EASY = 1;
const uint8_t DIFFICULTY_NORMAL = 2;
const uint8_t DIFFICULTY_HEROIC = 3;
const uint8_t DIFFICULTY_LEGENDARY = 4;

const MessageType timeMessages[] = { MessageType::Time1, MessageType::Time2, MessageType::Time3, MessageType::Time4 };

struct ChatMessage
{
  uint8_t sender;
  MessageType message;
  uint8_t data;
};



class Chat
{
  private:
    short me;
    HardwareSerial *serial;

  public:
    Chat(ChatSource id);
    void begin();
    void send(MessageType message, uint8_t data = 0);
    bool receive(ChatMessage* message);
    void debug(String s);
};

#endif

