#include "control/send-action.hpp"
#include "radio.hpp"
#include "util.hpp"
#include "setup/device-id.hpp"

void send_action(ActionChecksum* checksum) {
  byte payload[] = {
    0x89, 0x26, 0x82, // header
    0x00, 0x00, 0x00, 0x00, // device id
    0xE0, 0x00, // seq
    0x00, // op
    0x00, 0x00, 0x00 // checksum
  };

  write_u32_be(&payload[3], device_id);

  payload[7] |= (checksum->seq) >> 3;
  payload[8] |= (checksum->seq & 0b111) << 5;
  payload[9] = checksum->op;

  memcpy(&payload[10], checksum->checksum, 3);

  radio.write(&payload, sizeof(payload));
}