#include "control/control.hpp"
#include "control/send-action.hpp"
#include "setup/collect-checksums.hpp"

static bool receive_input = true;

enum Key: u8 {
  ARROW_1 = 27,
  ARROW_2 = 91,
  SPACE = 32,
  ARROW_UP = 65,
  ARROW_DOWN = 66,
  ARROW_RIGHT = 67,
  ARROW_LEFT = 68,
};
static void on_key(Key key);

static Action key_to_action(Key key) {
  switch (key) {
    case Key::SPACE: return Action::TOGGLE;
    case Key::ARROW_RIGHT: return Action::BRIGHTNESS_UP;
    case Key::ARROW_LEFT: return Action::BRIGHTNESS_DOWN;
    case Key::ARROW_UP: return Action::COLOR_COLD;
    case Key::ARROW_DOWN: return Action::COLOR_WARM;
    default: return Action::UNKNOWN;
  }
}

void control_init() {
  printf(
    "\033[2J\033[H Successfully initialised.\n"
    "  - Brightness: arrow key left/right\n"
    "  - Color: arrow key up/down\n"
    "  - Toggle: space\n"
  );

  while (receive_input) {
    while (!Serial.available()) {
      yield();
    }

    Key key = static_cast<Key>(Serial.read());
    if (key == Key::ARROW_1 || key == Key::ARROW_2) {
      continue;
    }

    on_key(key);
  }
}

static u8 previous_seq = 0;
static void on_key(Key key) {
  Action action = key_to_action(key);
  if (action == Action::UNKNOWN) return;

  std::vector<ActionChecksum>* checksums = &action_checksums.at(action);
  ActionChecksum* checksum = &(checksums->at(0));
  if (checksum->seq == previous_seq) {
    checksum = &(checksums->at(1));
  }

  previous_seq = checksum->seq;

  send_action(checksum);
}
