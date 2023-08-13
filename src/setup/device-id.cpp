#include "setup/device-id.hpp"
#include "setup/main.hpp"
#include "setup/collect-checksums.hpp"

u32 device_id = 0x55be661f;

void device_id_init() {
  printf(
    "\033[2J\033[H Step 1: Collecting device ID\n"
    "  - Twist your remote back and forth to generate payloads.\n\n"
  );
}

void device_id_process() {
  static u8 successes = 0;
  static u8 fails = 0;
  static u32 candidate = 0;

  u32 id = payload.device_id();
  if (candidate == 0) {
    candidate = id;
  }

  printf(
    "\033[H Step 1: Collecting device ID\n"
    "  - Evaluating candidate device ID \"%X\" with %02.2f%% certainty\n"
    "  - Processed %u/%u (%02.2f%%) payloads. Keep twisting your remote!\n\n",
    candidate, (successes * 100.0) / double(successes + fails),
    successes, 0xFF, (successes * 100.0) / double(0xFF)
  );

  if (id == candidate) {
    successes++;
  } else {
    fails++;
  }

  if (successes == 0xFF) {
    device_id = id;
    return collect_checksums_init();
  } else if (fails > (successes * 3)) {
    candidate = id;
    fails = 0;
    successes = 0;
  }
}

bool device_id_completed() {
  return device_id != 0x00000000;
}
