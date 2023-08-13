#include "setup/collect-checksums.hpp"
#include "setup/main.hpp"
#include "setup/verify-checksums.hpp"
#include "util.hpp"

#define NUM_CHECKSUMS 2

std::unordered_map<Action, std::vector<ActionChecksum>> action_checksums(sizeof(ACTIONS));

static void print_progress() {
  String progress =
    "\033[H Step 2: Collecting checksums\n"
    "  - Perform the commands below until their goals are met (2/2).\n";

  for (Action action : ACTIONS) {
    progress += "  - " + Payload::action_to_string(action) + ": ";
    progress += String(action_checksums[action].size()) + "/" + String(NUM_CHECKSUMS);
    progress += "\n";
  }

  Serial.println(progress);
}

void collect_checksums_init() {
  if (action_checksums.empty()) {
    for (Action action : ACTIONS) {
      action_checksums[action] = std::vector<ActionChecksum>();
    }
  }

  printf("\033[2J\n");
  print_progress();
}

static bool is_seq_available(u8 seq) {
  for (Action action : ACTIONS) {
    for (ActionChecksum checksum : action_checksums.at(action)) {
      if (checksum.seq == seq) return false;
    }
  }

  return true;
}

void collect_checksums_process() {
  Action action = payload.action();
  u8* checksum = payload.checksum();
  u8 seq = payload.seq();

  if (!is_seq_available(seq)) {
    return;
  }

  std::vector<ActionChecksum>* checksums = &action_checksums.at(action);
  if (checksums->size() == NUM_CHECKSUMS) {
    return;
  }

  checksums->push_back(ActionChecksum {
    .seq = seq,
    .op = payload.op_raw(),
    .checksum = { checksum[0], checksum[1], checksum[2] },
    .verified = false,
  });

  if (collect_checksums_completed()) {
    return verify_checksums_init();
  }

  print_progress();
}

bool collect_checksums_completed() {
  for (Action action : ACTIONS) {
    std::vector<ActionChecksum>* checksums = &action_checksums.at(action);
    if (checksums->size() != NUM_CHECKSUMS) {
      return false;
    }
  }

  return true;
}