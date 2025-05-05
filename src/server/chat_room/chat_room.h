#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include <array>
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../../common.h"
#include "../participant/participant.h"

class chatRoom {
 public:
  void enter(std::shared_ptr<Participant> participant,
             const std::string& nickname);
  void leave(std::shared_ptr<Participant> participant);
  void broadcast(std::array<char, MAX_MSG_SIZE>& msg,
                 std::shared_ptr<Participant> participant);

 private:
  static constexpr size_t max_recent_msgs = 100;
  std::unordered_set<std::shared_ptr<Participant>> participants_;
  std::unordered_map<std::shared_ptr<Participant>, std::string> name_table_;
  std::deque<std::array<char, MAX_MSG_SIZE>> recent_msgs_;
};

#endif
