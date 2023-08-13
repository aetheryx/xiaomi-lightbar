#include <Arduino.h>
#pragma once

#define PAYLOAD_SIZE 13

enum Field: u8 {
  SHORT_PRESS = 0,
  PRESSED_ROTATE = 1,
  ROTATE = 2,
  LONG_PRESS = 3
};

enum Direction: u8 {
  RIGHT = 0,
  LEFT = 1
};

enum Action: u8 {
  COLOR_COLD = (Field::PRESSED_ROTATE << 1) | Direction::RIGHT,
  COLOR_WARM = (Field::PRESSED_ROTATE << 1) | Direction::LEFT,
  BRIGHTNESS_UP = (Field::ROTATE << 1) | Direction::RIGHT,
  BRIGHTNESS_DOWN = (Field::ROTATE << 1) | Direction::LEFT,
  TOGGLE = (Field::SHORT_PRESS << 1) | Direction::LEFT,
  PRESET = (Field::LONG_PRESS << 1) | Direction::RIGHT,
  UNKNOWN = 0xFF,
};
const Action ACTIONS[] = {
  COLOR_WARM, COLOR_COLD, BRIGHTNESS_DOWN, BRIGHTNESS_UP, TOGGLE
  // PRESET
};

class Payload {
  private:
    byte* data;

  public:
    Payload(void* data);

    u32 device_id();
    u16 seq();
    u8 op_raw();
    u8* checksum();

    Field field();
    Direction direction();
    Action action();

    String to_string();
    static String action_to_string(Action action);
};
