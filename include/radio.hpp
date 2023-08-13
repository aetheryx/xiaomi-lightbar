#pragma once
#include <RF24.h>

class Radio: public RF24 {
  public:
    Radio();
    void open();
    void configure();
    void write(void* payload, u8 length);
};

static Radio radio;
