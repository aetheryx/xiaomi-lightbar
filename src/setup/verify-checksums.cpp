#include "setup/verify-checksums.hpp"
#include "setup/device-id.hpp"
#include "util.hpp"
#include "radio.hpp"

static String action_precondition(Action action) {
  switch (action) {
    case Action::BRIGHTNESS_DOWN: return "Make sure your lamp's brightness is all the way up.";
    case Action::BRIGHTNESS_UP: return "Make sure your lamp's brightness is all the way down.";
    case Action::COLOR_COLD: return "Make sure your lamp's color is set to the warmest setting, and brightness is all the way up.";

    case Action::COLOR_WARM:
    case Action::PRESET:
      return "Make sure your lamp's color is set to the coldest setting, and brightness is all the way up.";

    default:
      return "";
  }
}

void verify_checksums_init() {
  for (Action action : ACTIONS) {
    printf(
      "\033[2J\033[H Step 3: Verifying checksums\n"
      "  - Verifying command: %s\n"
      "  - First: %s\n",
      Payload::action_to_string(action).c_str(),
      action_precondition(action).c_str()
    );

    confirm("  - Ready? (y/n) ");

    printf("\n  - Broadcasting payloads...\n");

    for (int i = 0; i < 16; i++) {
      ActionChecksum ac = action_checksums.at(action).at(i % 2);
      byte payload[] = {
        0x89, 0x26, 0x82, // header
        0x00, 0x00, 0x00, 0x00, // device id
        0xE0, 0x00, // seq
        0x00, // op
        0x00, 0x00, 0x00 // checksum
      };

      write_u32_be(&payload[3], device_id);

      payload[7] |= ac.seq >> 3;
      payload[8] |= (ac.seq & 0b111) << 5;
      payload[9] = ac.op;

      memcpy(&payload[10], ac.checksum, 3);

      radio.write(&payload, sizeof(payload));
      delay(100);
    }

    confirm("  - Did your device respond? (y/n) ");
  }
}