#include <Arduino.h>

u32 read_u32_be(byte* data);
void write_u32_be(byte* data, u32 value);

char ask(const char* prompt);
void confirm(const char* prompt);
