#include "setup/verify-checksums.hpp"
#include "setup/device-id.hpp"
#include "control/control.hpp"
#include "control/send-action.hpp"
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

static void verify_checksum(Action action);

void verify_checksums_init() {
  radio.stopListening();

  for (Action action : ACTIONS) {
    printf(
      "\033[2J\033[H Step 3: Verifying checksums\n"
      "  - Verifying command: %s\n"
      "  - First: %s\n",
      Payload::action_to_string(action).c_str(),
      action_precondition(action).c_str()
    );

    confirm("  - Ready? (y/n) ");
    verify_checksum(action);
  }

  if (!collect_checksums_completed()) {
    return collect_checksums_init();
  } else {
    return control_init();
  }
}

static void verify_checksum(Action action) {
  printf("\n  - Broadcasting payloads...\n");
  std::vector<ActionChecksum>* checksums = &action_checksums.at(action);

  for (int i = 0; i < 16; i++) {
    ActionChecksum* checksum = &checksums->at(i % 2);
    send_action(checksum);

    delay(action == Action::TOGGLE ? 500 : 10);
    yield();
  }

  char c = ask("  - Did your device respond? (y/n) ");
  if (c != 'y') {
    checksums->clear();
  }
}