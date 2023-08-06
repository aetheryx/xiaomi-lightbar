#include <Arduino.h>
#include <RF24.h>

/*

SPI Speedz      = 10 Mhz
STATUS          = 0x0e RX_DR=0 TX_DS=0 MAX_RT=0 RX_P_NO=7 TX_FULL=0
RX_ADDR_P0-1    = 0xaa67229ba3 0xc2c2c2c2c2
RX_ADDR_P2-5    = 0xc3 0xc4 0xc5 0xc6
TX_ADDR         = 0xaa67229ba3
RX_PW_P0-6      = 0x0d 0x0d 0x0d 0x0d 0x0d 0x0d
EN_AA           = 0x00
EN_RXADDR       = 0x03
RF_CH           = 0x44
RF_SETUP        = 0x0b
CONFIG          = 0x06
DYNPD/FEATURE   = 0x00 0x00
Data Rate       = 2 MBPS
Model           = nRF24L01+
CRC Length      = Disabled
PA Power        = PA_LOW
ARC             = 0

SPI Speedz      = 10 Mhz
STATUS          = 0x0e RX_DR=0 TX_DS=0 MAX_RT=0 RX_P_NO=7 TX_FULL=0
RX_ADDR_P0-1    = 0xaa67229ba3 0xc2c2c2c2c2
RX_ADDR_P2-5    = 0xc3 0xc4 0xc5 0xc6
TX_ADDR         = 0xaa67229ba3
RX_PW_P0-6      = 0x0d 0x0d 0x0d 0x0d 0x0d 0x0d
EN_AA           = 0x00
EN_RXADDR       = 0x03
RF_CH           = 0x44
RF_SETUP        = 0x0b
CONFIG          = 0x06
DYNPD/FEATURE   = 0x00 0x00
Data Rate       = 2 MBPS
Model           = nRF24L01+
CRC Length      = Disabled
PA Power        = PA_LOW
ARC             = 0

*/

const u8 address_width = 5;
const u8 address[address_width] = { 0xa3, 0x9B, 0x22, 0x67, 0xAA };
const u8 channel = 68;
static RF24 radio(PIN_RADIO_CE, PIN_RADIO_CSN);

void writer_setup() {
  if (!radio.begin()) {
    while (1) {
      printf("radio failed to initialise\n");
    }
  }

  radio.openWritingPipe(address);
  radio.disableDynamicPayloads();

  radio.setAutoAck(false);
  radio.disableCRC();
  radio.setChannel(channel);
  radio.setDataRate(RF24_2MBPS);
  radio.setAddressWidth(address_width);
  radio.setPayloadSize(13);

  radio.printDetails();

  delay(1000);

  byte payload[] = {
    0x89, 0x26, 0x82, // ?
    0x55, 0xBE, 0x66, 0x1F, // device id
    0xED, 0xA0, // seq
    0x20, // op (toggle)
    0b00111011, 0b00110110, 0b110 << 5 // checksum
  };

  radio.setPALevel(RF24_PA_LOW);
  for (u8 i = 0; i < 32; i++) {
    radio.write(&payload, sizeof(payload), true);
    delay(20);
    yield();
    printf("it\n");
  }
}

void writer_loop() {
}
