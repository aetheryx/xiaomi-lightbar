#include <Arduino.h>
#include "payload.hpp"

struct ActionChecksum {
  u8 seq;
  u32 checksum;
};

extern std::unordered_map<Action, std::vector<ActionChecksum>> action_checksums;

void init_checksums();
bool checksums_done();
void handle_checksum();