#include <RF24.h>
#include "radio.hpp"

const u8 address_width = 5;
const u8 address[address_width] = { 0xA3, 0x9B, 0x22, 0x67, 0xAA };
const u8 channel = 68;

Radio::Radio(): RF24(PIN_RADIO_CE, PIN_RADIO_CSN) {
}

void Radio::configure() {
  if (!this->begin()) {
    printf("radio failed to initialise\n");
    while (1);
  }

  this->setChannel(channel);
  this->setDataRate(RF24_2MBPS);
  this->setAutoAck(false);
  this->setAddressWidth(address_width);
  this->setCRCLength(RF24_CRC_DISABLED);
  this->disableAckPayload();
  this->disableDynamicPayloads();
}

void Radio::open_rx() {
  this->openReadingPipe(0, address);
  this->startListening();
  radio.printDetails();
}

void Radio::open_tx() {
}