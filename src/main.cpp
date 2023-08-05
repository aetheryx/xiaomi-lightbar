#include <Arduino.h>

u32 counter = 0;

void setup() {
  Serial.begin(MONITOR_SPEED);
  printf("\n\nstart\n");
}

void loop() {
  printf("%d\n", counter++);
  delay(100);
}
