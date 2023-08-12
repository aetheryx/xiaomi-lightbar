#include "setup/action-checksums.hpp"
#include "setup/main.hpp"

std::unordered_map<Action, std::vector<ActionChecksum>> action_checksums(sizeof(ACTIONS));

void init_checksums() {
  for (Action action : ACTIONS) {
    action_checksums[action] = std::vector<ActionChecksum>();
  }
}

bool checksums_done() {
  for (Action action : ACTIONS) {
    std::vector<ActionChecksum>* checksums = &action_checksums.at(action);
    if (checksums->size() != 4) {
      return false;
    }
  }

  return true;
}

static bool is_seq_available(u8 seq) {
  for (Action action : ACTIONS) {
    for (ActionChecksum checksum : action_checksums.at(action)) {
      if (checksum.seq == seq) return false;
    }
  }

  return true;
}

void handle_checksum() {
  Action action = payload.action();
  u32 checksum = payload.checksum();
  u8 seq = payload.seq();

  if (!is_seq_available(seq)) {
    printf("skipping occupied seq %d\n", seq);
    return;
  }

  std::vector<ActionChecksum>* checksums = &action_checksums.at(action);
  if (checksums->size() == 4) {
    printf("skipping completed action %d\n", action);
    return;
  }

  printf("wrote checksum %u %u %u\n", action, seq, checksum);
  ActionChecksum action_checksum = {
    .seq = seq,
    .checksum = checksum,
  };

  checksums->push_back(action_checksum);
  printf("size after = %d\n", checksums->size());
}