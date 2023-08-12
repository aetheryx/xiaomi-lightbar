#include "setup/main.hpp"
#include "setup/device-id.hpp"
#include "setup/action-checksums.hpp"

byte buf[PAYLOAD_SIZE];
Payload payload(&buf[3]);

void setup_main() {
  radio.configure();
  radio.open_rx();
  init_checksums();
}

void setup_loop() {
  if (!radio.available()) {
    return;
  }

  radio.read(&buf, PAYLOAD_SIZE);

  char raw_hex[(PAYLOAD_SIZE * 3)];
  for (int i = 0; i < PAYLOAD_SIZE; i++) {
    sprintf(&raw_hex[i * 3], "%02X ", buf[i]);
  }
  raw_hex[(PAYLOAD_SIZE * 3) - 1] = '\0';

  printf(
    "device_id=%x seq=%03u to_string=%s cs=%x\n",
    payload.device_id(),
    payload.seq(),
    payload.to_string().c_str(),
    payload.checksum()
  );


  if (device_id == 0) {
    return handle_device_id();
  }

  if (payload.device_id() != device_id) {
    return;
  }

  if (!checksums_done()) {
    return handle_checksum();
  }
}