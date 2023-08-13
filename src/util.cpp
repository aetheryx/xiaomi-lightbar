#include "util.hpp"

u32 read_u32_be(byte* data) {
  return (
    (data[0] << 24) |
    (data[1] << 16) |
    (data[2] << 8) |
    (data[3] << 0)
  );
}

void write_u32_be(byte* data, u32 value) {
  u8* value_ptr = reinterpret_cast<u8*>(&value);
  data[0] = value_ptr[3];
  data[1] = value_ptr[2];
  data[2] = value_ptr[1];
  data[3] = value_ptr[0];
}

char ask(const char* prompt) {
  printf(prompt);
  fflush(stdout);
  while (!Serial.available()) {
    yield();
  }

  return Serial.read();
}

void confirm(const char* prompt) {
  char c = ask(prompt);
  if (c != 'y') {
    printf("Halted.\n");
    while (1) yield();
  }
}
