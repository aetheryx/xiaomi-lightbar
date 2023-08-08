#include <Arduino.h>
#include "radio.hpp"
#include "payload.hpp"
#include "listener.hpp"

void listener_setup() {
  radio.configure();
  radio.open_rx();
}

void listener_loop() {
  if (!radio.available()) {
    return;
  }

  static byte buf[PAYLOAD_SIZE];
  static Payload payload(&buf[3]);

  radio.read(&buf, PAYLOAD_SIZE);

  char raw_hex[(PAYLOAD_SIZE * 3)];
  for (int i = 0; i < PAYLOAD_SIZE; i++) {
    sprintf(&raw_hex[i * 3], "%02X ", buf[i]);
  }
  raw_hex[(PAYLOAD_SIZE * 3) - 1] = '\0';

  printf(
    "device_id=%x seq=%03u field=%02u direction=%u action=%03u to_string=%s raw=[%s]\n",
    payload.device_id(),
    payload.seq(),
    payload.field(),
    payload.direction(),
    payload.action(),
    payload.to_string().c_str(),
    raw_hex
  );
}
