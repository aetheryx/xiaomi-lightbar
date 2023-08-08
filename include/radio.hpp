#include <RF24.h>

class Radio: public RF24 {
  public:
    Radio();
    void open_rx();
    void open_tx();
    void configure();
};

static Radio radio;
