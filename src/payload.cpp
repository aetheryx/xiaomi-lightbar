#include <Arduino.h>
#include "payload.hpp"

/**
 * payload spec:
 * the remote uses DPL, but the encoded payload length always seems to be 9 bytes. any 
 * payloads I was able to intercept that had other sizes, just seemed to have bit errors 
 * in the 6-bit payload length field, and garbage after the 9th byte
 * 
 * bits [0, 8): constant 0x4
 *    - padding for 9-bit packet control field?
 * 
 * bits [8, 40): device id
 *    - n.b.: I'm not exactly sure where the boundaries between the first constant byte
 *      and the device ID are, because I only have two devices to test. might begin
 *      before or after bit 8, or there might be more constant data before bit 40
 * 
 * bits [42, 50): packet sequence number
 *    - used to dedupe payloads on the receiver. (E)SB acks don't seem to be enabled,
 *      the remote just sends a bunch of payloads for each action and the receiver 
 *      dedupes them
 * 
 * bits [55, 64): command
 *    - bits [55, 56]: action (see enum Action in payload.hpp)
 *    - bit 57: direction (see enum Direction in payload.hpp)
 *        - when there's no rotation: short press = high, long press = low
 *    - bits [58, 64)
 *        - more or less garbage data
 *        - for rotating actions, it's actually the entire range of bits [57, 64) that 
 *          contain the direction (all 0x0 or 0x7F), but I only read the first bit in 
 *          this range (57). rotationless actions will forcibly set bit 57, but
 *          the subsequent bits in [58, 64) retain what they were set to previously
 *          (i.e. the direction of the previous rotating action)
 * 
 * bits [64, 72): crc probably
*/

Payload::Payload(void* data) {
  this->data = reinterpret_cast<byte*>(data);
}

u32 Payload::device_id() {
  return (data[1] << 24) | (data[2] << 16) | (data[3] << 8) | data[4];
}

u16 Payload::seq() {
  u16 seq_high = (data[5] & ~(0b11 << 6)) << 2;
  u16 seq_low = data[6] >> 6;
  return seq_high | seq_low;
}

Action Payload::action() {
  u8 action = ((data[6] & 0b1) << 1) | (data[7] >> 7);
  return static_cast<Action>(action);
}

Direction Payload::direction() {
  u8 direction = (data[7] >> 6) & 0b1;
  return static_cast<Direction>(direction);
}

String Payload::to_string() {
  Direction direction = this->direction();

  switch (action()) {
    case Action::ROTATE:
      return "BRIGHTNESS_" + String(direction == Direction::RIGHT ? "UP" : "DOWN");

    case Action::PRESSED_ROTATE:
      return "COLOR_" + String(direction == Direction::RIGHT ? "COLD" : "WARM");

    case Action::SHORT_PRESS:
      return "TOGGLE";
    case Action::LONG_PRESS:
      return "PRESET";

    default:
      return "UNKNOWN";
  }
}