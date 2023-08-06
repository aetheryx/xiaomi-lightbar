#include <Arduino.h>

#define PAYLOAD_SIZE 9

enum Action: u8 {
  SHORT_PRESS = 0,
  PRESSED_ROTATE = 1,
  ROTATE = 2,
  LONG_PRESS = 3
};

enum Direction: u8 {
  RIGHT = 0,
  LEFT = 1
};

class Payload {
  private:
    byte* data;

  public:
    Payload(void* data);

    u32 device_id();
    u16 seq();
    Action action();
    Direction direction();
    String to_string();
};
