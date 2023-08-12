#include <Arduino.h>
#include <printf.h>
#include "setup/main.hpp"

void setup() {
  Serial.begin(MONITOR_SPEED);
  printf_begin();

  setup_main();
}


void loop() {
  setup_loop();
}

