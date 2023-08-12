#include <Arduino.h>
#include "payload.hpp"

/**
 * payload spec
 *  - byte 0: preamble
 *    - constant 0xAA
 *  - bytes [1, 5]: RF address
 *    - address width: 5 bytes
 *    - constant 0x67, 0x22, 0x9B, 0xA3, 0x89
 *  - bytes [6, 7]: PIDF
 *    - constant 0x26, 0x82 (payload size 9, packet ID 0b10, ack bit 0b1)
 *  - bytes [8, 14]: payload
 *    - bytes [0, 3]: device ID
 *    - bytes [4, 5]: packet seq
 *    - bytes [6]: op
 *        - bits [0, 1]: field (long press, short press, rotate, rotate while held down)
 *        - bit [2]: direction (left, right)
 *  - bytes [15, 18]: checksum
*/

static u32 read_u32(byte* data) {
  return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}

Payload::Payload(void* data) {
  this->data = reinterpret_cast<byte*>(data);
}

u32 Payload::device_id() {
  return read_u32(&data[0]);
}

u16 Payload::seq() {
  u16 seq_high = (data[4] & ~0xE0) << 3;
  u16 seq_low = data[5] >> 5;

  return seq_high | seq_low;
}

u8 Payload::op_raw() {
  return data[6];
}

u32 Payload::checksum() {
  return read_u32(&data[7]);
}

Field Payload::field() {
  u8 field = op_raw() >> 6;
  return static_cast<Field>(field);
}

Direction Payload::direction() {
  u8 direction = (op_raw() >> 5) & 0b1;
  return static_cast<Direction>(direction);
}

Action Payload::action() {
  u8 action = op_raw() >> 5;
  return static_cast<Action>(action);
}

String Payload::to_string() {
  switch (action()) {
    case Action::BRIGHTNESS_UP: return "BRIGHTNESS_UP";
    case Action::BRIGHTNESS_DOWN: return "BRIGHTNESS_DOWN";
    case Action::COLOR_COLD: return "COLOR_COLD";
    case Action::COLOR_WARM: return "COLOR_WARM";
    case Action::PRESET: return "PRESET";
    case Action::TOGGLE: return "TOGGLE";

    default:
      return "UNKNOWN";
  }
}