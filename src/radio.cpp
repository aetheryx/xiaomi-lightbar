#include <RF24.h>
#include "radio.hpp"

#define RETRIES 8

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
  this->setRetries(0, 0);
}

void Radio::open() {
  this->openReadingPipe(0, address);
  this->startListening();

  this->openWritingPipe(address);
  this->setPALevel(rf24_pa_dbm_e::RF24_PA_LOW);
  this->setPayloadSize(13);

  radio.printDetails();
}

void Radio::write(void* payload, u8 length) {
  radio.stopListening();

  for (u8 i = 0; i < RETRIES; i++) {
    this->writeFast(payload, length, true);
    delay(20);
    yield();
  }
}