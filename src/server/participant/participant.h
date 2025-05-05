#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <array>
#include <memory>

#include "../../common.h"

class Participant {
 public:
  virtual ~Participant() = default;
  virtual void onMessage(std::array<char, MAX_MSG_SIZE>& msg) = 0;
};

#endif
