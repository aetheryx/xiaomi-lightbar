#pragma once
#include <Arduino.h>
#include "payload.hpp"

struct ActionChecksum {
  u8 seq;
  u8 op;
  byte checksum[3];
  bool verified;
};

extern std::unordered_map<Action, std::vector<ActionChecksum>> action_checksums;

void collect_checksums_init();
bool collect_checksums_completed();
void collect_checksums_process();
