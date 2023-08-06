#include <Arduino.h>
#include <RF24.h>
#include "payload.hpp"

const u64 address = 0x67229BA389;
const u8 channel = 68;
static RF24 radio(PIN_RADIO_CE, PIN_RADIO_CSN);

void listener_setup() {
  if (!radio.begin()) {
    while (1) {
      printf("radio failed to initialise\n");
    }
  }

  radio.openReadingPipe(0, address);

  radio.setChannel(channel);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(false);
  radio.setAddressWidth(5); 
  radio.setCRCLength(RF24_CRC_DISABLED);
  radio.disableAckPayload(); 
  radio.enableDynamicPayloads();

  radio.closeReadingPipe(1);
  radio.startListening();
  radio.printPrettyDetails();
}

static byte buf[PAYLOAD_SIZE];
static Payload payload(&buf);

void listener_loop() {
  if (!radio.available()) {
    return;
  }

  u8 payload_size = radio.getDynamicPayloadSize();
  if (payload_size != PAYLOAD_SIZE) {
    radio.flush_rx();
    return;
  }

  radio.read(&buf, payload_size);

  char raw_hex[(payload_size * 3)];
  for (int i = 0; i < payload_size; i++) {
    sprintf(&raw_hex[i * 3], "%02X ", buf[i]);
  }
  raw_hex[(payload_size * 3) - 1] = '\0';

  printf(
    "t=%07.3f raw=[%s] device_id=%x seq=%03d op=(%u, %d, %s)\n",
    millis() / 1000.0f,
    raw_hex,
    payload.device_id(),
    payload.seq(),
    payload.action(),
    payload.direction(),
    payload.to_string().c_str()
  );
}

