#include "setup/device-id.hpp"
#include "setup/main.hpp"

u32 device_id = 0x55be661f;

void handle_device_id() {
  static u8 successes = 0;
  static u8 fails = 0;
  static u32 candidate = 0;

  u32 id = payload.device_id();
  if (candidate == 0) {
    candidate = id;
  }

  printf(
    "handle_device_id(): successes=%d fails=%d candidate=%x id=%x\n",
    successes, fails, candidate, id
  );

  if (id == candidate) {
    successes++;
  } else {
    fails++;
  }

  if (successes == 0xFF) {
    device_id = id;
  } else if (fails > (successes * 3)) {
    candidate = id;
    fails = 0;
    successes = 0;
  }
}