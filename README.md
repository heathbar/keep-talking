# Keep Talking Game

This project is an arduino-based, real-world version of the video game "Keep Talking and Nobody Explodes".

{% include youtube.html id="scMQZVPicUo" %}

## Serial Protocol

The devices are connnected in a loop using the hardware serial pins on each device. Any module can send a message at any time to the next module. The receiver echoes the message on to the next module, then processes the message. When a message has been successfully echoed around the entire loop, the original sending module receives back it's own message. It recognizes that it was the sender of the message and can safely ignore the message.

All messages are 4 bytes, with the exception of the Debug message type. 

| byte 0 | byte 1 | byte 2 | byte 3 |
|--------|--------|--------|--------|
| header | source | type   | data   |


### Source

- 0x00 Debug
- 0x01 Main
- 0x02 Simon Says
- 0x03 Password
- 0x04 Buttons
- 0x05 Clock
- 0x06 Wires
- 0x07 Hieroglyphs
- 0x08 BinaryCounter
- 0x09 Maze
- 0x10 Keypad
- 0x11 Mastermind
- 0x12 TBD


### Event

- 0x00 Debug
- 0x01 Reset
- 0x02 Disarm
- 0x03 Strike
- 0x04 Detonate
- 0x05 TimeQuery
- 0x06 Time1
- 0x07 Time2
- 0x08 Time3
- 0x09 Time4
- 0x10 Win
- 0x11 Quote


### Reset Flags

- 0x01  SN_CONTAINS_VOWEL
- 0x02  SN_LAST_DIGIT_ODD
- 0x04  IND_CTRL
- 0x08  IND_ALT
- 0x016 IND_ETP