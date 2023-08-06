#include <Arduino.h>
#include <SPI.h>
#include <printf.h>
#include "listener.hpp"

void setup() {
  Serial.begin(MONITOR_SPEED);
  printf_begin();

  printf("\n\nstart\n");

  listener_setup();
}

void loop() {
  listener_loop();
}
