#include "setup/main.hpp"
#include "setup/device-id.hpp"
#include "setup/collect-checksums.hpp"
#include "util.hpp"

byte buf[PAYLOAD_SIZE];
Payload payload(&buf[3]);

void setup_main() {
  radio.configure();
  radio.open();

  collect_checksums_init();
}

void setup_loop() {
  if (!radio.available()) {
    return;
  }

  radio.read(&buf, PAYLOAD_SIZE);

  // char* raw_hex = new char[(sizeof(payload) * 3)];
  // for (int i = 0; i < PAYLOAD_SIZE; i++) {
  //   sprintf(&raw_hex[i * 3], "%02X ", buf[i]);
  // }
  // raw_hex[(PAYLOAD_SIZE * 3) - 1] = '\0';

  // printf(
  //   "device_id=%x seq=%03u to_string=%s raw=[%s]\n",
  //   payload.device_id(),
  //   payload.seq(),
  //   payload.to_string().c_str(),
  //   raw_hex
  // );

  if (!device_id_completed()) {
    return device_id_process();
  }

  if (payload.device_id() != device_id) {
    return;
  }

  if (!collect_checksums_completed()) {
    return collect_checksums_process();
  }
}